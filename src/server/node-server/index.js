import express from "express";
import { readFile } from "fs/promises"
import { WebSocketServer } from "ws";

const server = express();
const websocket = new WebSocketServer({
    port: 7000
});

const root = "../..";

server.use(express.static(`${root}/client`));
server.get("/", async (req, res) => {
    console.log("!?")
    const contents = await readFile(`${root}/client/index.html`, 'utf-8')
        .catch(console.error);
    console.log(contents);
    res.send(contents);
})

websocket.on('connection', ws => {
    console.log('New client connected!')
    ws.send('connection established')
    ws.on('close', () => console.log('Client has disconnected!'))
    ws.on('message', data => {
        sockserver.clients.forEach(client => {
            console.log(`distributing message: ${data}`)
            client.send(`${data}`)
        })
    })
    ws.onerror = console.error;
})

server.listen(3000, () => console.log("node-server started!"));