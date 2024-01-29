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
        heart.style.animationDuration = Math.random() * 2 + 3 + "s"; // Random animation duration
        heart.innerText = "❤️";
        document.body.appendChild(heart);

        // Remove heart after it falls down
        setTimeout(() => {
            heart.remove();
        }, 5000);
    }
});

