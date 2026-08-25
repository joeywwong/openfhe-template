#pragma once

#include "openfhe.h"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace openfhe_starter {

struct CkksOptions {
    std::uint32_t multiplicativeDepth{3};
    std::uint32_t scalingModulusBits{50};
    std::uint32_t batchSize{8};
    lbcrypto::SecurityLevel securityLevel{lbcrypto::HEStd_128_classic};
};

// Creates a standard CKKS context and enables the features needed by the basic
// example. Project-specific code can extend this function or replace it.
lbcrypto::CryptoContext<lbcrypto::DCRTPoly> CreateCkksContext(
    const CkksOptions& options = {});

// Decrypts an approximate CKKS vector and trims it to the requested length.
std::vector<double> DecryptVector(
    const lbcrypto::CryptoContext<lbcrypto::DCRTPoly>& context,
    const lbcrypto::PrivateKey<lbcrypto::DCRTPoly>& secretKey,
    const lbcrypto::Ciphertext<lbcrypto::DCRTPoly>& ciphertext,
    std::size_t outputLength);

}  // namespace openfhe_starter

