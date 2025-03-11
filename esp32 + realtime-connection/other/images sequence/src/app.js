let selected = 1;
document.getElementById(selected).style.opacity = "1";

document.addEventListener("keydown", function (event) {
  selected = event.key;

  let selections = document.querySelectorAll(".section");
  selections.forEach(function (section) {
    section.style.opacity = "0";
  });
  document.getElementById(selected).style.opacity = "1";
});
