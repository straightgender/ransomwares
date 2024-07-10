require 'openssl'
require 'securerandom'
require 'fileutils'

def generate_key
  SecureRandom.random_bytes(32)
end

def pad_data(data)
  block_size = 16
  padding_length = block_size - (data.bytesize % block_size)
  data + padding_length.chr * padding_length
end

def encrypt_file(file_path, key)
  iv = SecureRandom.random_bytes(16)
  cipher = OpenSSL::Cipher::AES256.new(:CBC)
  cipher.encrypt
  cipher.key = key
  cipher.iv = iv

  data = File.read(file_path, mode: 'rb')
  padded_data = pad_data(data)
  encrypted_data = cipher.update(padded_data) + cipher.final

  File.write(file_path, iv + encrypted_data, mode: 'wb')
end

def process_folder(folder_path, key)
  Dir.glob("#{folder_path}/*").each do |file_path|
    if File.file?(file_path)
      encrypt_file(file_path, key)
    end
  end
end

def main
  folder_path = "E:/Test/Folder"
  parent_path = File.dirname(folder_path)
  key_file_path = File.join(parent_path, "key.txt")

  key = generate_key
  File.write(key_file_path, key, mode: 'wb')

  process_folder(folder_path, key)
end

main
