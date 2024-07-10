import java.io.File
import java.nio.file.Files
import java.nio.file.Paths
import javax.crypto.Cipher
import javax.crypto.KeyGenerator
import javax.crypto.SecretKey
import javax.crypto.spec.IvParameterSpec
import javax.crypto.spec.SecretKeySpec

fun generateKey(): SecretKey {
    val keyGen = KeyGenerator.getInstance("AES")
    keyGen.init(256)
    return keyGen.generateKey()
}

fun padData(data: ByteArray): ByteArray {
    val blockSize = 16
    val paddingLength = blockSize - (data.size % blockSize)
    return data + ByteArray(paddingLength) { paddingLength.toByte() }
}

fun encryptFile(filePath: String, key: SecretKey) {
    val cipher = Cipher.getInstance("AES/CBC/PKCS5Padding")
    val iv = ByteArray(16)
    Files.newInputStream(Paths.get(filePath)).use { it.read(iv) }
    val ivSpec = IvParameterSpec(iv)
    cipher.init(Cipher.ENCRYPT_MODE, key, ivSpec)

    val data = Files.readAllBytes(Paths.get(filePath))
    val paddedData = padData(data)
    val encryptedData = cipher.doFinal(paddedData)

    Files.write(Paths.get(filePath), iv + encryptedData)
}

fun processFolder(folderPath: String, key: SecretKey) {
    File(folderPath).listFiles()?.forEach { file ->
        if (file.isFile) {
            encryptFile(file.path, key)
        }
    }
}

fun main() {
    val folderPath = "E:/Test/Folder"
    val parentPath = File(folderPath).parent
    val keyFilePath = Paths.get(parentPath, "key.txt")

    val key = generateKey()
    Files.write(keyFilePath, key.encoded)

    processFolder(folderPath, key)
}
