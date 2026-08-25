#include "openfhe_starter/ckks_context.hpp"

#include <stdexcept>

namespace openfhe_starter {

lbcrypto::CryptoContext<lbcrypto::DCRTPoly> CreateCkksContext(const CkksOptions& options) {
    if (options.multiplicativeDepth == 0 || options.batchSize == 0) {
        throw std::invalid_argument("CKKS depth and batch size must be positive");
    }

    lbcrypto::CCParams<lbcrypto::CryptoContextCKKSRNS> parameters;
    parameters.SetMultiplicativeDepth(options.multiplicativeDepth);
    parameters.SetScalingModSize(options.scalingModulusBits);
    parameters.SetBatchSize(options.batchSize);
    parameters.SetSecurityLevel(options.securityLevel);

    auto context = lbcrypto::GenCryptoContext(parameters);
    context->Enable(lbcrypto::PKE);
    context->Enable(lbcrypto::KEYSWITCH);
    context->Enable(lbcrypto::LEVELEDSHE);
    // EvalSum and other packed linear-algebra helpers are part of ADVANCEDSHE.
    context->Enable(lbcrypto::ADVANCEDSHE);
    return context;
}

std::vector<double> DecryptVector(
    const lbcrypto::CryptoContext<lbcrypto::DCRTPoly>& context,
    const lbcrypto::PrivateKey<lbcrypto::DCRTPoly>& secretKey,
    const lbcrypto::Ciphertext<lbcrypto::DCRTPoly>& ciphertext,
    std::size_t outputLength) {
    lbcrypto::Plaintext plaintext;
    const auto result = context->Decrypt(secretKey, ciphertext, &plaintext);
    if (!result.isValid) {
        throw std::runtime_error("OpenFHE decryption failed");
    }
    plaintext->SetLength(outputLength);
    auto values = plaintext->GetRealPackedValue();
    values.resize(outputLength);
    return values;
}

}  // namespace openfhe_starter
