var path = '../icons/icons_animals';
// function setUserIcon() {
//
// }
function setUserIcon() {
    if (document.getElementById("icon-container").style.display == "none") {
        document.getElementById("icon-container").style.display = "block";
    } else {
        document.getElementById("icon-container").style.display = "none";
    }
    // Get the container element where the icons will be displayed
    var iconContainer = document.getElementById("icon-container");

    if (!iconContainer.hasChildNodes()) {

        // An array to store the icons
        var icons = [];
        for (i = 0; i < 8; i++) {
            // Create an img element for each icon
            var img = document.createElement("img");
            // Set the src and alt attributes for the img element
            img.src = path + `/${i}.svg`;
            img.setAttribute("style", "width: 25px; height: 25px;")
            // img.alt = icon.alt;
            // Add a click event to the img element
            img.addEventListener("click", function () {
                // Set the user icon to the clicked img element
                setUserIcon(this);
            });
            // Add the img element to the icon container
            iconContainer.appendChild(img);
            // Add the icon to the icons array
            icons.push(img);
        }

        // Function to set the user icon
        function setUserIcon(icon) {
            let menu = document.getElementById("userIcon");
            menu.replaceChild(icon, menu.firstElementChild);

            iconContainer.setAttribute("style", "display: none;")
        }
    }
}