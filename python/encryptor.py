import os
import pathlib
import random
import string
from cryptography.hazmat.primitives import padding
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
from cryptography.hazmat.primitives.kdf.scrypt import Scrypt
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.scrypt import Scrypt
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
from cryptography.hazmat.primitives.kdf.scrypt import Scrypt
from cryptography.hazmat.primitives.kdf.scrypt import Scrypt
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC

def generate_key():
    return os.urandom(32)

def pad_data(data):
    padder = padding.PKCS7(algorithms.AES.block_size).padder()
    padded_data = padder.update(data) + padder.finalize()
    return padded_data

def encrypt_file(file_path, key):
    backend = default_backend()
    iv = os.urandom(16)
    cipher = Cipher(algorithms.AES(key), modes.CBC(iv), backend=backend)
    encryptor = cipher.encryptor()

    with open(file_path, 'rb') as f:
        data = f.read()

    padded_data = pad_data(data)
    encrypted_data = encryptor.update(padded_data) + encryptor.finalize()

    with open(file_path, 'wb') as f:
        f.write(iv + encrypted_data)

def process_folder(folder_path, key):
    for entry in pathlib.Path(folder_path).iterdir():
        if entry.is_file():
            encrypt_file(entry, key)

def main():
    folder_path = "D:\\Test\\Docs"
    parent_path = pathlib.Path(folder_path).parent
    key_file_path = parent_path / "key.txt"

    key = generate_key()

    with open(key_file_path, 'wb') as f:
        f.write(key)

    process_folder(folder_path, key)

if __name__ == "__main__":
    main()
