#include "openfhe_starter/ckks_context.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {

void PrintVector(const char* label, const std::vector<double>& values) {
    std::cout << std::left << std::setw(18) << label << '[';
    for (std::size_t i = 0; i < values.size(); ++i) {
        std::cout << (i == 0 ? "" : ", ") << std::fixed << std::setprecision(4) << values[i];
    }
    std::cout << "]\n";
}

}  // namespace

int main() {
    try {
        constexpr std::uint32_t slots = 8;
        auto context = openfhe_starter::CreateCkksContext({3, 50, slots});
        auto keyPair = context->KeyGen();
        if (!keyPair.good()) {
            throw std::runtime_error("OpenFHE key generation failed");
        }

        // Evaluation keys permit selected ciphertext operations. They do not
        // disclose the secret key or permit decryption.
        context->EvalMultKeyGen(keyPair.secretKey);
        context->EvalRotateKeyGen(keyPair.secretKey, {1, -1});
        context->EvalSumKeyGen(keyPair.secretKey);

        const std::vector<double> x{0.25, -0.50, 0.75, 1.00, 0.0, 0.0, 0.0, 0.0};
        const std::vector<double> y{2.00, 1.50, -1.00, 0.50, 0.0, 0.0, 0.0, 0.0};
        auto plainX = context->MakeCKKSPackedPlaintext(x);
        auto plainY = context->MakeCKKSPackedPlaintext(y);
        auto encryptedX = context->Encrypt(keyPair.publicKey, plainX);
        auto encryptedY = context->Encrypt(keyPair.publicKey, plainY);

        // All four computations below operate on ciphertexts. Decryption is
        // deliberately kept at the boundary where results are displayed.
        auto encryptedAdd = context->EvalAdd(encryptedX, encryptedY);
        auto encryptedMultiply = context->EvalMult(encryptedX, encryptedY);
        auto encryptedRotate = context->EvalRotate(encryptedX, 1);
        auto encryptedSum = context->EvalSum(encryptedX, slots);

        std::cout << "OpenFHE CKKS starter\n";
        PrintVector("x + y", openfhe_starter::DecryptVector(
                                    context, keyPair.secretKey, encryptedAdd, 4));
        PrintVector("x * y", openfhe_starter::DecryptVector(
                                    context, keyPair.secretKey, encryptedMultiply, 4));
        PrintVector("rotate(x, 1)", openfhe_starter::DecryptVector(
                                           context, keyPair.secretKey, encryptedRotate, 4));
        PrintVector("sum(x)", openfhe_starter::DecryptVector(
                                   context, keyPair.secretKey, encryptedSum, 1));
        return 0;
    }
    catch (const std::exception& exception) {
        std::cerr << "error: " << exception.what() << '\n';
        return 1;
    }
}

