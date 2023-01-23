var server = new SillyClient();
var url = "wss://ecv-etic.upf.edu/node/9000/ws";

server.on_connect = function () {
    console.log("connected");
    server.getReport(function (report) {
        Object.keys(report["rooms"]).forEach(room => {
            updateChatList(room);
        });
        renderChatList(chatList[0]);
    });
};

function handleNotification(text) {
    var chat = document.getElementById("chat");

    var notification = document.createElement('div');
    notification.setAttribute("class", "notification");
    var d = new Date(); // for now
    notification.innerHTML = `${text} at ${d.getHours()}:${(d.getMinutes() < 10 ? '0' : '') + d.getMinutes()}`;
    chat.appendChild(notification);
}


// type: text, history
var content = {
    // "data": [{
    // "type": data.type,
    // "content": data.content,
    // "user_id": user_id,
    // "username": data.username,
    // }]
}
var chatList = [];
var currentGroupName = "";
var myUserID = null;
var myUserName = "default username";
var iconID = null
var activeUsers = null;

function setOnlineUsers(number) {
    document.getElementById("connected-users").innerHTML = `${number} user online`;
}

server.on_room_info = function (info) {
    activeUsers = info['clients']["length"];
    setOnlineUsers(activeUsers);
};

function setUserName(event) {
    if (event.keyCode == 13) {
        document.getElementById("body").setAttribute("style", "pointer-events : auto");
        document.getElementById("username-enter").style.display = "none";
        myUserName = document.getElementById("username-input").value;
    }
}

server.on_message = function (author_id, data) {
    updateChatList(currentGroupName);
    appendHistoryContent(author_id, data);
    console.log(data);
    var dataJSON = JSON.parse(data);
    if (Object.hasOwn(dataJSON, "content")) {
        if (Object.hasOwn(dataJSON, "username")) {
            console.log(dataJSON["username"])
            MYCHAT.createMessage(dataJSON["content"], dataJSON["username"], author_id);
        } else {
            console.log(author_id)
            MYCHAT.createMessage(dataJSON["content"], author_id, author_id);
        }
    }
}

function updateChatList(currentGroupName) {
    if (!chatList.includes(currentGroupName)) {
        chatList.unshift(currentGroupName);
        return true;
    } else {
        return false;
    }
}

function appendHistoryContent(user_id, data) {
    console.log(data);
    data = JSON.parse(data);
    let appendData = {
        "data": [{
            "type": data.type,
            "content": data.content,
            "user_id": user_id,
            "username": data.username,
        }]
    };
    // content.push(appendData);
}

async function renderChatList(currentGroupName) {
    if (!updateChatList(currentGroupName)) {
        chatList.splice(chatList.indexOf(currentGroupName), 1);
        updateChatList(currentGroupName);
        console.log("group exists");
    }
    document.getElementById("chat-list").innerHTML = '';
    chatList.forEach(chat => {
        var message = 'No messages';
        let createAt = '';
        var li = document.createElement("div");
        li.setAttribute("class", "group-box")

        li.innerHTML = '        <div class="group">' +
            '             <div class="group-icon">' +
            '             <img class="group-icon" src="../icons/icons_utils/group-svgrepo-com.svg">' +
            '        </div>' +
            '            <div class="details">' +
            `                 <h2>${chat}</h2>` +
            '                <h3>' +
            `                    ${message}` +
            '                </h3>' +
            '            </div>' +
            '        </div>'
        document.getElementById("chat-list").appendChild(li);
        li.addEventListener("click", async () => {
            console.log("click room");
            document.getElementById("chat").innerHTML = '';
            closeConnection();
            await server.connect(url, currentGroupName);
            server.on_ready = function (id) {
                server.getRoomInfo(currentGroupName, (data) => {
                    userList = data.clients;
                    console.log("change server");
                    console.log(userList);
                })
            }

        });
    });
    document.getElementById("chat-area").setAttribute("style", "display: block");
}

async function setNewGroup(event) {
    currentGroupName = document.getElementById("group input").value
    if (event.keyCode == 13 && currentGroupName != "" && updateChatList(currentGroupName)) {
        await server.connect(url, currentGroupName);
        server.on_ready = function (id) {
            myUserID = id;
            server.getRoomInfo(currentGroupName, async (res) => {
                userList = await res.clients;
                console.log(userList)
                // if (addDataToChatList(currentGroupName)) {
                renderChatList(currentGroupName);
                // }

            })
        };
    }

}

async function closeConnection() {
    return await server.close();
}

async function sendLog(log, user_id) {
    await server.sendMessage(log, [user_id]);
}

//this methods is called when a new user is connected
server.on_user_connected = function (user_id) {
    sendLog(content, user_id);
    handleNotification(user_id + ' joins');
    activeUsers += 1;
    setOnlineUsers(activeUsers);
}

//this methods is called when a user leaves the room
server.on_user_disconnected = function (user_id) {
    handleNotification(user_id + ' leaves');
    activeUsers -= 1;
    setOnlineUsers(activeUsers);
}

server.on_close = function () {
    //server closed
    console.log("server is closed");
};

//this method is called when coulndt connect to the server
server.on_error = function (err) {
    console.error(err);
};

