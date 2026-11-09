#pragma once

#include <string>
#include <vector>
#include <optional>
#include <cstdint>

namespace Vexgram::PQC {

// Константы протокола квантового шифрования поверх Telegram (Overlay PQC)
constexpr auto kPqcMagicPrefix = "[VEXGRAM:PQC:v1:";
constexpr auto kPqcMagicSuffix = "]";

enum class Algorithm {
    ML_KEM_768 = 1, // NIST FIPS 203 (Kyber-768)
    ML_KEM_1024 = 2,
    HYBRID_X25519_KYBER768 = 3 // Рекомендуемый гибридный режим
};

struct KeyPair {
    Algorithm algorithm;
    std::vector<uint8_t> publicKey;
    std::vector<uint8_t> privateKey;
};

struct EncryptedPayload {
    Algorithm algorithm;
    std::vector<uint8_t> encapsulation; // KEM Ciphertext
    std::vector<uint8_t> iv;            // 12 bytes AES-GCM IV
    std::vector<uint8_t> tag;           // 16 bytes auth tag
    std::vector<uint8_t> ciphertext;    // Encrypted message body
};

class Engine {
public:
    static Engine &Instance();

    // Генерация PQC пары ключей для профиля
    KeyPair GenerateKeyPair(Algorithm algo = Algorithm::HYBRID_X25519_KYBER768);

    // Зашифровать открытый текст в безопасный контейнер
    std::string EncryptMessage(const std::string &plainText, const std::vector<uint8_t> &recipientPublicKey);

    // Попытка расшифровать сообщение (если содержит kPqcMagicPrefix)
    std::optional<std::string> DecryptMessage(const std::string &messageBody, const std::vector<uint8_t> &myPrivateKey);

    // Проверка, является ли текст квантово-защищенным
    bool IsPqcMessage(const std::string &text) const;

private:
    Engine() = default;
};

} // namespace Vexgram::PQC
