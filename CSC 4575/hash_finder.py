import re
import urllib3
import os
import requests
import argparse
import concurrent.futures
import websocket
 
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

parser = argparse.ArgumentParser()
parser.add_argument('-s', help='hash', dest='hash')
parser.add_argument('-f', help='file containing hashes', dest='file')
parser.add_argument('-d', help='directory containing hashes', dest='dir')
parser.add_argument('-t', help='number of threads', dest='threads', type=int)

args = parser.parse_args()
found = 0
hashv = ''
directory = args.dir
file = args.file
thread_count = args.threads or 4
 
if directory and directory[-1] == '/':
    directory = directory[:-1]
 
def alpha(hashvalue, hashtype):
    headers = {
        'User-Agent': 'Mozilla/5.0',
        'Content-Type': 'application/x-www-form-urlencoded',
    }

    data = {
        '__EVENTTARGET': 'Button1',
        'ctl00$ContentPlaceHolder1$TextBoxInput': hashvalue,
        'ctl00$ContentPlaceHolder1$InputHashType': hashtype,
        'ctl00$ContentPlaceHolder1$Button1': 'decrypt',
    }

    response = requests.post('https://www.cmd5.org/', headers=headers, data=data)
    match = re.search(r'<span id="LabelAnswer"[^>]+?>(.+)</span>', response.text)
    return match.group(1) if match else False

def send_message(ws, message):
    global found, hashv
    ws.send(message)
    response = ws.recv()
    match = re.search(r'"value\\":\\([^,]+)', response)

    if match:
        found = 1
        hashv = response[148:match.end()-2]
        return hashv
 
def beta(hashvalue, hashtype):
    ws = websocket.create_connection("wss://md5hashing.net/sockjs/697/etstxji0/websocket")
    send_message(ws, r'["{\"msg\":\"connect\",\"version\":\"1\",\"support\":[\"1\",\"pre2\",\"pre1\"]}"]')
    method_message = r'["{\"msg\":\"method\",\"method\":\"hash.get\",\"params\":[\"{}\",\"{}\"],\"id\":\"1\"}"]'.format(hashtype, hashvalue)
    send_message(ws, method_message)
    return hashv if found else False
 
def gamma(hashvalue, _):
    response = requests.get(f'https://www.nitrxgen.net/md5db/{hashvalue}', verify=False).text
    return response if response else False
 
def theta(hashvalue, hashtype):
    response = requests.get(f'https://md5decrypt.net/Api/api.php?hash={hashvalue}&hash_type={hashtype}&email=email@example.com&code=apikey').text
    return response if response else False
 
md5 = [alpha, beta, gamma, theta]
sha1 = sha256 = sha384 = sha512 = [alpha, beta, theta]
 
def crack(hashvalue):
    hash_lengths = {
        32: ('md5', md5),
        40: ('sha1', sha1),
        64: ('sha256', sha256),
        96: ('sha384', sha384),
        128: ('sha512', sha512)
    }

    if len(hashvalue) in hash_lengths:
        hashtype, apis = hash_lengths[len(hashvalue)]
        for api in apis:
            r = api(hashvalue, hashtype)
            if r:
                return r
    return False
 
result = {}
 
def threaded(hashvalue):
    resp = crack(hashvalue)
    if resp:
        print(f'{hashvalue} : {resp}')
        result[hashvalue] = resp
 
def grepper(directory):
    os.system(f'grep -Pr "[a-f0-9]{{32,128}}" {directory} --exclude=*.{{png,jpg,jpeg,mp3,mp4,zip,gz}} > {directory.split("/")[-1]}.txt')
 
def miner(file):
    found = set()
    with open(file, 'r') as f:
        for line in f:
            found.update(re.findall(r'[a-f0-9]{32,128}', line))

    threadpool = concurrent.futures.ThreadPoolExecutor(max_workers=thread_count)
    futures = [threadpool.submit(threaded, hashvalue) for hashvalue in found]
    for _ in concurrent.futures.as_completed(futures):
        pass
 
def single(args):
    result = crack(args.hash)
    print(result if result else "Hash not found.")
 
if directory:
    grepper(directory)

elif file:
    miner(file)
    with open(f'cracked-{file.split("/")[-1]}', 'w') as f:
        for hashvalue, cracked in result.items():
            f.write(f'{hashvalue}:{cracked}\n')

elif args.hash:
    single(args)