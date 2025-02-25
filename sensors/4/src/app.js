const fingers = ["1 👆", "2 🤙", "3 🤟", "4 🤘🤘", "5 🖐"];

document.body.addEventListener("touchstart", function (e) {
  let count = e.touches.length;
  document.body.textContent = fingers[count - 1];
});

document.body.addEventListener("touchend", function (e) {
  let count = e.touches.length;
  document.body.textContent = count + "👊";
});

document.body.addEventListener("touchmove", function (e) {
  let count = e.touches.length;
  document.body.textContent = fingers[count - 1];
});
