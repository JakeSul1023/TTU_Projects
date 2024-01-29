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
