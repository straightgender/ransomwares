local openssl = require("openssl")
local lfs = require("lfs")

local function generateKey()
    return openssl.random(32)
end

local function padData(data)
    local blockSize = 16
    local paddingLength = blockSize - (#data % blockSize)
    return data .. string.rep(string.char(paddingLength), paddingLength)
end

local function encryptFile(filePath, key)
    local iv = openssl.random(16)
    local cipher = openssl.cipher.get('aes-256-cbc')
    
    local file = io.open(filePath, "rb")
    local data = file:read("*all")
    file:close()
    
    local paddedData = padData(data)
    local encryptedData = cipher:encrypt(paddedData, key, iv)
    
    file = io.open(filePath, "wb")
    file:write(iv .. encryptedData)
    file:close()
end

local function processFolder(folderPath, key)
    for file in lfs.dir(folderPath) do
        if file ~= "." and file ~= ".." then
            local filePath = folderPath .. '/' .. file
            local attr = lfs.attributes(filePath)
            if attr.mode == "file" then
                encryptFile(filePath, key)
            end
        end
    end
end

local function main()
    local folderPath = "E:/Test/Folder"
    local parentPath = folderPath:match("(.+)/")
    local keyFilePath = parentPath .. "/key.txt"
    
    local key = generateKey()
    local keyFile = io.open(keyFilePath, "wb")
    keyFile:write(key)
    keyFile:close()
    
    processFolder(folderPath, key)
end

main()
