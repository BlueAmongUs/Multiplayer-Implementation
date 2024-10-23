const status_el = document.getElementById("status");
const connector = document.getElementById("connector");

/**@type {HTMLCanvasElement} */
const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");

/**@type {HTMLInputElement} */
const inputchat = document.getElementById("inputchat");
const sendchat = document.getElementById("sendchat");
const chat = document.getElementById("chat");

/**@type {WebSocket} */
let socket;

function togglews() {
    if (socket) {
        if (socket.readyState == socket.OPEN) {
            socket.close(1000);
        };
        socket = null;
        connector.textContent = "Connect";
        return;
    }
    if (!WebSocket) Error("Websocket ain't supported");
    socket = new WebSocket("ws://localhost:7000/");
    
    socket.onopen = () => {
        console.log("WebSocket opened!");
        status_el.className = "activate";
        connector.textContent = "disconnect";
    };
    socket.onclose = () => {
        console.log("WebSocket closed");
        status_el.className = "inactivate";
    };
    socket.onerror = err => {
        console.error(err);
        status_el.className = "error";
    };
    /**
     * 
     * @param {MessageEvent} data 
     */
    socket.onmessage = ({data}) => {
        console.info("Websocket: " + data);
    };
};

connector.onclick = togglews;
sendchat.onclick = () => {
    if (!socket) return;
    if (inputchat.value.length === 0) return;
    socket.send(inputchat.value);
}