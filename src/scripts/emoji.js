// Get the container element where the table will be placed
var container = document.getElementById("emoji-table-container");
// Create the table element
var table = document.createElement("table");
// Create the table header row
var headerRow = document.createElement("tr");
var emojiHeader = document.createElement("th");

headerRow.appendChild(emojiHeader);
table.appendChild(headerRow);

// Read the emoji data from a txt file
fetch('../src/scripts/emoji.txt')
    .then(response => response.text())
    .then(data => {
        // Split the data by new line
        var lines = data.split('\n');
        var row = document.createElement("tr");
        for (var i = 0; i < lines.length; i++) {
            // Create a new row for each emoji
            if (i % 20 == 0) {
                if (row) {
                    table.appendChild(row);
                }
                var row = document.createElement("tr");
            }
            var emojiCell = document.createElement("td");
            // Split the line by space
            var line = lines[i].split(" ");
            // Get the first word
            // Append the emoji to the cell
            var emojiCode = line[0];
            emojiCell.innerHTML = `&#x` + emojiCode + `;`;
            // console.log(emojiCell)
            emojiCell.setAttribute("onclick", `pickEmoji('\\u{${emojiCode}}')`);
            row.appendChild(emojiCell);
            // Create the description cell
            var descriptionCell = document.createElement("td");
        }
        // Append the table to the container element
        document.getElementById("emoji-table-container").style.display = "none";
        container.appendChild(table);
    });

function chooseEmoji() {
    if (document.getElementById("emoji-table-container").style.display == "none") {
        document.getElementById("emoji-table-container").style.display = "block";
    } else {
        document.getElementById("emoji-table-container").style.display = "none";
    }
}

function pickEmoji(emoji) {
    var inputArea = document.getElementById("input area");
    var text = inputArea.value;
    text = text + emoji;
    console.log(emoji);
    inputArea.value = text;
}