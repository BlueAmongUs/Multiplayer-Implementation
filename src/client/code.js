const canvas = document.getElementById("canvas");
const status_el = document.getElementById("status");

const socket = new WebSocket("ws://localhost:5500/","multiplayer");

socket.onopen = () => {
    console.log("WebSocket opened!");
    status_el.className = "activate";
};
socket.onclose = () => {
    console.log("WebSocket closed");
    status_el.className = "inactivate";
};
socket.onerror = err => {
    console.error(err);
    status_el.className = "error";
};

socket.onmessage = (data) => {
    console.log("Websocket: " + data);
};