var net = require('net');
var fs = require('fs');

var HOST = '127.0.0.1';
var PORT = 6969;
var FILE = './src/data.json';

const interpretBinary = (bin: string) => {
    const name = bin.substr(0,6);
    const state = bin.substr(6,2);
    return {name: name, state: { danger: state === "00"}};
}

const update = (name: string, state: any) => {
    fs.readFile(FILE, (err:any, buf:any) => {
        console.log(buf);
        const data = JSON.parse(buf.toString());
        data[name].state = state;
        fs.writeFile(FILE, JSON.stringify(data), (err:any, data:any) => {
            if (err) console.log(err);
            console.log("Successfully Written to File.");
        });
    });
}

net.createServer((sock: any) => {
    console.log('CONNECTED: ' + sock.remoteAddress +':'+ sock.remotePort);
    sock.on('data', (input:any) => {

        if(input) {
            const bin = input.toString('utf8');
    
            console.log('DATA ' + sock.remoteAddress + ': ' + bin);
            
            const { name, state } = interpretBinary(bin);
    
            update(name, state);

        }
    });

    sock.on('close', (data:any) => {
        console.log('CLOSED: ' + sock.remoteAddress +' '+ sock.remotePort);
    });
    
}).listen(PORT, HOST);

console.log('Server listening on ' + HOST +':'+ PORT);