"""
Author: Jacob Sullivan
Date: 08/21/2024
Description: This project <Vulnerability Spider> was a self-driven summer project
focused on developing a web scraping tool combined with some simple vulneribility 
detection capabilities. The core objective is to identify common words that could lead to 
potential security issues on websites by leveraging scraping techniques and 
analyzying web content for known vulnerabilites.
"""
import scrapy
import os
from scrapy_splash import SplashRequest
import logging
from urllib.parse import urlparse

"""
Custom logging handler to capute log messages. 
Stores them in a list for later usage.
"""
class CaptureLogsHandler(logging.Handler):
    def __init__(self):
        super().__init__()
        self.logs = []
        self.setFormatter(logging.Formatter('%(message)s'))

    def emit(self, record):
        self.logs.append(self.format(record))
"""
This is a spider for scraping web pages and identifying potential vulnerbilities
using scrapy and splash to handle dynamic content and perform checks.
"""
class VulnSpider(scrapy.Spider):
    name = 'spider_vuln'
    
    start_urls = [
        'https://www.tntech.edu/'   #URL to be scraped
    ]
    
    """
    Initializes the spider with custom logging and vulnerability indicators.
    """
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        
        # Custom logger to keep track of log messages
        self.log_handler = CaptureLogsHandler()
        self.custom_logger = logging.getLogger('spider_vuln_logger')
        self.custom_logger.setLevel(logging.INFO)
        self.custom_logger.addHandler(self.log_handler)
        self.custom_logger.propagate = False

        # Defining keywords that might indicate vulnerabilities
        self.vuln_indicators = {
            'A02': ['md5', 'sha1', 'plaintext', 'base64', 'rsa', 'aes', 'decrypt', 'ssl', 'tls', 'https', 'secure'],
            'A03': ['<script', 'eval(', '?'],
            'A04': ['insecure'],
            'A05': ['<meta name="description"', 'config', 'Index of'],
            'A06': ['cdn', 'cookieconsent.min.js'],
            'A07': ['login', 'password', 'signin', 'auth', 'username', 'session', 'remember me', '2fa', 'mfa', 'credentials', 'auth-token', 'csrf-token'],
            'A08': ['integrity', 'checksum', 'hash', 'signature', 'signed', 'verify', 'tamper', 'validation', 'sri'],
            'A09': ['logging', 'audit', 'tracking', 'monitoring'],
            'A10': ['request', 'fetch', 'http']
        }
        self.auth_headers = ['Authorization', 'WWW-Authenticate']

    """
    Initiates requests to the URLs listed in the <starts_url>.
    It uses SplashRequest to handle JavaScript and dynamic content.
    """
    def start_requests(self):
        for url in self.start_urls:
            self.custom_logger.info(f"Starting request for: {url}") # Logging the URL we are starting with 
            yield SplashRequest(
                url,
                callback=self.parse,
                args={
                    'lua_source': self.splash_script(),
                    'wait': 2
                }
            )

    """
    Provides a Lua script for Splash to execute. 
    This script loads the page and waits for it to be ready, 
    and clicks on all links to scrape further content
    """
    def splash_script(self):
        return '''
        function main(splash, args)
            splash:on_request(function(request)
                -- You can log requests here if needed
            end)

            splash:go(args.url)
            splash:wait(2) -- Wait for the page to load

            -- Click on all links on the page
            local links = splash:select_all('a')
            for _, link in ipairs(links) do
                link:mouse_click()
                splash:wait(2) -- Wait for the page to load after clicking
            end

            -- Optionally, you can return the HTML content or other information
            return {
                html = splash:html(),
                url = splash:url()
            }
        end
        '''

    """
    This processes the response from the request. 
    Then extracts the page details and checks for vulnerbilities,
    and follows more resources and links within the same domain.
    """
    def parse(self, response):
        self.custom_logger.info(f"Parsing URL: {response.url}")

        try:
            title_page = response.xpath('//title/text()').get(default='No title')
            body_page = response.body.decode('utf-8')
            cookies = response.headers.getlist('Set-Cookie')
            cookies_str = [cookie.decode('utf-8') for cookie in cookies]

            logs = [
                f"Scraping URL: {response.url}",
                f"Page title: {title_page[:100]}...",
                f"Page body (first 500 chars): {body_page[:500]}...",
                f"Response headers: {dict(response.headers)}",
                f"Cookies: {cookies_str}"
            ]

            # Check vulnerabilities
            self.check_vulnerabilities(response, body_page, logs)
            
            # Checks for authentication headers and insecure cookies
            self.check_authentication_headers(response, logs)
            self.check_cookies(cookies, logs)

            formatted_logs = " :: ".join(logs)

            yield {
                'url': response.url,
                'title': title_page,
                'body_snippet': body_page[:500],
                'cookies': cookies_str,
                'formatted_logs': formatted_logs
            }

            # Follow additional resources: scripts, images, stylesheets, etc.
            resource_selectors = {
                'script': '//script/@src',
                'img': '//img/@src',
                'link': '//link[@rel="stylesheet"]/@href'
            }
            
            base_domain = urlparse(self.start_urls[0]).netloc
            
            for resource_type, selector in resource_selectors.items():
                for resource_url in response.xpath(selector).getall():
                    absolute_url = response.urljoin(resource_url)
                    url_domain = urlparse(absolute_url).netloc
                    
                    if url_domain == base_domain:
                        yield SplashRequest(absolute_url, callback=self.parse_resource, args={'wait': 2}, meta={'resource_type': resource_type})
                    else:
                        self.custom_logger.info(f"Skipping external resource URL: {absolute_url}")

            # Follow links on the page within the same domain
            for href in response.css('a::attr(href)').getall():
                absolute_url = response.urljoin(href)
                url_domain = urlparse(absolute_url).netloc
                
                if url_domain == base_domain:
                    yield SplashRequest(absolute_url, callback=self.parse, args={'wait': 2})
                else:
                    self.custom_logger.info(f"Skipping external URL: {absolute_url}")

        except Exception as e:
            self.custom_logger.error(f"Error processing {response.url}: {e}")

    """
    Processes the additional resources found on the page.
    Logs the resources details and checks for vulnerbilities in the scripts.
    """
    def parse_resource(self, response):
        self.custom_logger.info(f"Parsing resource: {response.url}")

        resource_type = response.meta['resource_type']
        resource_content = response.body.decode('utf-8', errors='ignore')

        logs = [
            f"Scraping {resource_type.upper()} resource: {response.url}",
            f"Resource size: {len(response.body)} bytes"
        ]

        # Check for vulnerabilities in scripts
        if resource_type == 'script':
            self.check_vulnerabilities(response, resource_content, logs)

        # Logs the resource processing
        self.custom_logger.info(" :: ".join(logs))

        yield {
            'resource_url': response.url,
            'resource_type': resource_type,
            'resource_size': len(response.body),
            'logs': " :: ".join(logs)
        }
        
    """
    Checks the page content for known vulnerability indicators.
    Then updates with logs with any detected potential issues.
    """
    def check_vulnerabilities(self, response, body_page, logs):
        found_vulns = False
        try:
            for vuln_code, indicators in self.vuln_indicators.items():
                if any(keyword in body_page.lower() for keyword in indicators):
                    logs.append(f"OWASP {vuln_code}:2021 - Potential issue detected on {response.url}")
                    found_vulns = True
            
            if not found_vulns:
                logs.append(f"No vulnerabilities detected on {response.url}")
        except Exception as e:
            self.custom_logger.error(f"Error in check_vulnerabilities for {response.url}: {e}")
            
    """
    Checks for authentication related headers in the response.
    Will log if any are found, indicating potential security concerns.
    """
    def check_authentication_headers(self, response, logs):
        for header in self.auth_headers:
            if header in response.headers:
                logs.append(f"OWASP A07:2021 - Authentication header found: {header} on {response.url}")

    """
    Checks if cookies are missing in the security attributes.
    Logs if any are detected
    """
    def check_cookies(self, cookies, logs):
        for cookie in cookies:
            cookie_str = cookie.decode('utf-8')
            if 'Secure' not in cookie_str or 'HttpOnly' not in cookie_str:
                logs.append(f"OWASP A06:2021 - Insecure cookie found: {cookie_str}")