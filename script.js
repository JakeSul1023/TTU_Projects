document.getElementById("noButton").addEventListener("mouseover", function() {
    const button = this;
    const container = document.querySelector('.container');
    const containerRect = container.getBoundingClientRect();
    
    const newX = Math.floor(Math.random() * (containerRect.width - button.clientWidth));
    const newY = Math.floor(Math.random() * (containerRect.height - button.clientHeight));

    button.style.position = "absolute";
    button.style.left = `${newX}px`;
    button.style.top = `${newY}px`;
});
document.getElementById("yesButton").addEventListener("click", function() {
    for (let i = 0; i < 30; i++) {
        const heart = document.createElement("div");
        heart.className = "heart";
        heart.style.left = Math.random() * 100 + "vw";
        heart.style.top = Math.random() * 100 + "vh";
        heart.innerText = "❤️";
        document.body.appendChild(heart);
    }
});
document.getElementById("yesButton").addEventListener("click", function() {
    // Create an image element
    const image = document.createElement("img");
    image.src = "image.jpg"; // Replace with the path to your image
    image.className = "popup-image";

    // Create a text element
    const text = document.createElement("div");
    text.className = "overlay-text";
    text.textContent = "I LOVE YOU!\n  (i buy you coffee)"; // Replace with your text

    // Optional: Create a div to act as an overlay
    const overlay = document.createElement("div");
    overlay.className = "overlay";

    // Append the image and overlay to the body
    overlay.appendChild(image);
    overlay.appendChild(text);
    document.body.appendChild(overlay);

    // Close the pop-up when clicking on the overlay
    overlay.addEventListener("click", function() {
        document.body.removeChild(overlay);
    });
});



