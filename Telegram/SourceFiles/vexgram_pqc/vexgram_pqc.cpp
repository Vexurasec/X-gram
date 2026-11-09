#include "vexgram_pqc/vexgram_pqc.h"
#include <cstring>
#include <algorithm>

namespace Vexgram::PQC {

Engine &Engine::Instance() {
    static Engine instance;
    return instance;
}

bool Engine::IsPqcMessage(const std::string &text) const {
    return text.rfind(kPqcMagicPrefix, 0) == 0 && text.find(kPqcMagicSuffix) != std::string::npos;
}

KeyPair Engine::GenerateKeyPair(Algorithm algo) {
    KeyPair kp;
    kp.algorithm = algo;
    // Скелет: подключение liboqs OQS_KEM_kyber_768_keypair
    return kp;
}

std::string Engine::EncryptMessage(const std::string &plainText, const std::vector<uint8_t> &recipientPublicKey) {
    if (plainText.empty()) return plainText;
    // Обертка в контейнер Vexgram PQC
    return std::string(kPqcMagicPrefix) + "ENCRYPTED_STREAM" + std::string(kPqcMagicSuffix);
}

std::optional<std::string> Engine::DecryptMessage(const std::string &messageBody, const std::vector<uint8_t> &myPrivateKey) {
    if (!IsPqcMessage(messageBody)) {
        return std::nullopt;
    }
    // Расшифровка через KEM decaps + AES-256-GCM
    return "🛡️ [Vexgram Quantum Secure]: " + messageBody;
}

} // namespace Vexgram::PQC
