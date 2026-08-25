#include "openfhe_starter/ckks_context.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    try {
        // These sparse-slot parameters follow OpenFHE 1.1.2's official
        // bootstrapping example. The small manual ring dimension makes this a
        // practical demonstration, not a standard security claim.
        constexpr std::uint32_t slots = 8;
        const std::vector<std::uint32_t> levelBudget{3, 3};
        const std::vector<std::uint32_t> bsgsDimensions{0, 0};
        constexpr auto secretKeyDistribution = lbcrypto::UNIFORM_TERNARY;

        lbcrypto::CCParams<lbcrypto::CryptoContextCKKSRNS> parameters;
        parameters.SetSecretKeyDist(secretKeyDistribution);
        parameters.SetSecurityLevel(lbcrypto::HEStd_NotSet);
        parameters.SetRingDim(1U << 12U);
        parameters.SetKeySwitchTechnique(lbcrypto::HYBRID);
        parameters.SetNumLargeDigits(3);
        parameters.SetBatchSize(slots);
        parameters.SetScalingTechnique(lbcrypto::FLEXIBLEAUTO);
        parameters.SetScalingModSize(59);
        parameters.SetFirstModSize(60);

        const std::uint32_t bootstrapDepth =
            lbcrypto::FHECKKSRNS::GetBootstrapDepth(levelBudget, secretKeyDistribution);
        parameters.SetMultiplicativeDepth(bootstrapDepth + 4);

        auto context = lbcrypto::GenCryptoContext(parameters);
        context->Enable(lbcrypto::PKE);
        context->Enable(lbcrypto::KEYSWITCH);
        context->Enable(lbcrypto::LEVELEDSHE);
        context->Enable(lbcrypto::ADVANCEDSHE);
        context->Enable(lbcrypto::FHE);
        context->EvalBootstrapSetup(levelBudget, bsgsDimensions, slots);

        auto keyPair = context->KeyGen();
        if (!keyPair.good()) {
            throw std::runtime_error("OpenFHE key generation failed");
        }
        context->EvalMultKeyGen(keyPair.secretKey);
        context->EvalBootstrapKeyGen(keyPair.secretKey, slots);

        const std::vector<double> values{0.10, -0.20, 0.30, -0.40, 0.0, 0.0, 0.0, 0.0};
        auto plaintext = context->MakeCKKSPackedPlaintext(values, 1, 0, nullptr, slots);
        plaintext->SetLength(slots);
        auto ciphertext = context->Encrypt(keyPair.publicKey, plaintext);

        // EvalBootstrap refreshes the encrypted value without secret-key access.
        // Real applications call it after a computation has consumed levels.
        auto refreshed = context->EvalBootstrap(ciphertext);
        const auto decoded = openfhe_starter::DecryptVector(
            context, keyPair.secretKey, refreshed, values.size());

        std::cout << "Genuine CKKS EvalBootstrap result:\n[";
        for (std::size_t i = 0; i < decoded.size(); ++i) {
            std::cout << (i == 0 ? "" : ", ") << std::fixed << std::setprecision(6)
                      << decoded[i];
        }
        std::cout << "]\n";
        return 0;
    }
    catch (const std::exception& exception) {
        std::cerr << "error: " << exception.what() << '\n';
        return 1;
    }
}

