const fs = require('fs');
const path = require('path');
const crypto = require('crypto');

function generateKey() {
    return crypto.randomBytes(32);
}

function padData(data) {
    const blockSize = 16;
    const paddingLength = blockSize - (data.length % blockSize);
    const padding = Buffer.alloc(paddingLength, paddingLength);
    return Buffer.concat([data, padding]);
}

function encryptFile(filePath, key) {
    const iv = crypto.randomBytes(16);
    const cipher = crypto.createCipheriv('aes-256-cbc', key, iv);

    const data = fs.readFileSync(filePath);
    const paddedData = padData(data);
    const encryptedData = Buffer.concat([cipher.update(paddedData), cipher.final()]);

    fs.writeFileSync(filePath, Buffer.concat([iv, encryptedData]));
}

function processFolder(folderPath, key) {
    fs.readdirSync(folderPath).forEach(file => {
        const filePath = path.join(folderPath, file);
        if (fs.statSync(filePath).isFile()) {
            encryptFile(filePath, key);
        }
    });
}

function main() {
    const folderPath = "E:/Test/Folder";
    const parentPath = path.dirname(folderPath);
    const keyFilePath = path.join(parentPath, "key.txt");

    const key = generateKey();
    fs.writeFileSync(keyFilePath, key);

    processFolder(folderPath, key);
}

main();
