#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <iostream>
#include <string>

using std::string;

std::pair<string, int> encrypt_string(const string& key, const string& plaintext) {
    // Check if the key length is valid for AES-256 (32 bytes)
    if (key.length() != 32) {
        throw std::invalid_argument("Invalid key length. Key must be 32 bytes for AES-256.");
    }

    // Generate a random IV (Initialization Vector)
    unsigned char iv[AES_BLOCK_SIZE];
    if (RAND_bytes(iv, sizeof(iv)) != 1) {
        throw std::runtime_error("Failed to generate random IV.");
    }

    // Create and initialize the cipher context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (ctx == nullptr) {
        throw std::runtime_error("Failed to create cipher context.");
    }

    // Initialize encryption operation with AES-256, CBC mode, and the provided key and IV
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                           reinterpret_cast<const unsigned char*>(key.c_str()), iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize encryption operation.");
    }

    // Calculate the required ciphertext buffer size
    int ciphertextLen = plaintext.length() + AES_BLOCK_SIZE;
    unsigned char* ciphertext = new unsigned char[ciphertextLen];

    // Encrypt the plaintext
    int len;
    if (EVP_EncryptUpdate(ctx, ciphertext, &len,
                          reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.length()) != 1) {
        delete[] ciphertext;
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to encrypt plaintext.");
    }

    int ciphertextTotalLen = len;

    // Finalize the encryption process
    if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1) {
        delete[] ciphertext;
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize encryption process.");
    }

    ciphertextTotalLen += len;

    // Append the IV at the beginning of the ciphertext
    string encryptedText(reinterpret_cast<const char*>(iv), AES_BLOCK_SIZE);
    encryptedText += string(reinterpret_cast<const char*>(ciphertext), ciphertextTotalLen);

    // Clean up
    delete[] ciphertext;
    EVP_CIPHER_CTX_free(ctx);

    return {encryptedText, ciphertextTotalLen};
}

std::string base64Encode(const unsigned char* data, int length) {
    BIO* bmem = BIO_new(BIO_s_mem());
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO_push(b64, bmem);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(b64, data, length);
    BIO_flush(b64);

    BUF_MEM* bptr;
    BIO_get_mem_ptr(b64, &bptr);

    std::string result(bptr->data, bptr->length);
    BIO_free_all(b64);

    return result;
}

int main() {
    // string key = "01234567890123456789012345678901";
    string key = "abcdefghijklmnopqrstuvwxzy123456";
    string plaintext = "Hello, world_um";

    auto pair = encrypt_string(key, plaintext);
    string ciphertext = pair.first;
    int ciphertextLen = pair.second;

    string final_cipher = base64Encode(reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertextLen);

    std::cout << "Ciphertext: " << ciphertext << std::endl;
    std::cout << "Final: " << final_cipher << std::endl;

    return 0;
}