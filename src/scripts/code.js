var MYCHAT =
    {

        input: null,

        onKeyPress(event) {
            // Handle enter
            if (event.key === "Enter") {
                MYCHAT.sendMessage();
            }
        },

        init: function () {
            console.log("init!");
            MYCHAT.input = document.getElementById("input area")
            MYCHAT.input.addEventListener("keydown", MYCHAT.onKeyPress);

        },


        sendMessage() {
            // Get msg
            var userMessage = MYCHAT.input.value;
            MYCHAT.createMessage(userMessage, myUserName, myUserID);
            // Clear msg
            MYCHAT.input.value = '';
            var chat = document.getElementById("chat");
            chat.scrollTop = 100000;
        },

        createMessage(msg, userName, userID) {
            if (msg.trim().length != 0) {
                var who = document.createElement('li');
                who.setAttribute("class", "you");
                //only if we send message
                if (userID == myUserID) {
                    who.setAttribute("class", "me");
                    data = {
                        "type": "text",
                        "username": userName,
                        "content": msg,
                    }
                    server.sendMessage(JSON.stringify(data));
                    console.log("send message")
                }
                {
                    // Construction of child node
                    var message = document.createElement('div');
                    message.setAttribute("class", "message");
                    var username = document.createElement('h2');
                    username.setAttribute("class", "username");
                    username.innerHTML = userName;

                    var text = document.createElement('div');
                    text.innerHTML = msg;

                    var time = document.createElement('span');
                    time.setAttribute("class", "time");
                    var d = new Date(); // for now
                    time.innerHTML = `${d.getHours()}:${(d.getMinutes() < 10 ? '0' : '') + d.getMinutes()}`;
                    text.appendChild(time);

                    message.appendChild(username);
                    message.appendChild(text);

                    who.appendChild(message);
                }
                var test = document.getElementById('chat');
                test.appendChild(who);


                appendHistoryContent(myUserID, JSON.stringify(msg));
                updateChatList(currentGroupName);
            }
        },

    };

MYCHAT.init();

