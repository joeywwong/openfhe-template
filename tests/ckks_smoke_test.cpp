#include "openfhe_starter/ckks_context.hpp"

#include <cmath>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace {

void ExpectNear(double actual, double expected, double tolerance) {
    if (std::abs(actual - expected) > tolerance) {
        throw std::runtime_error("CKKS result exceeded the approximation tolerance");
    }
}

}  // namespace

int main() {
    try {
        auto context = openfhe_starter::CreateCkksContext();
        auto keyPair = context->KeyGen();
        if (!keyPair.good()) {
            throw std::runtime_error("OpenFHE key generation failed");
        }
        context->EvalMultKeyGen(keyPair.secretKey);
        context->EvalRotateKeyGen(keyPair.secretKey, {1});
        context->EvalSumKeyGen(keyPair.secretKey);

        const std::vector<double> x{0.25, -0.5, 0.75, 1.0, -1.25, 1.5, -1.75, 2.0};
        const std::vector<double> y{2.0, 1.5, -1.0, 0.5, 0.25, -0.5, 0.75, -1.5};
        auto encryptedX = context->Encrypt(
            keyPair.publicKey, context->MakeCKKSPackedPlaintext(x));
        auto encryptedY = context->Encrypt(
            keyPair.publicKey, context->MakeCKKSPackedPlaintext(y));

        const auto added = openfhe_starter::DecryptVector(
            context, keyPair.secretKey, context->EvalAdd(encryptedX, encryptedY), x.size());
        const auto multiplied = openfhe_starter::DecryptVector(
            context, keyPair.secretKey, context->EvalMult(encryptedX, encryptedY), x.size());
        const auto rotated = openfhe_starter::DecryptVector(
            context, keyPair.secretKey, context->EvalRotate(encryptedX, 1), x.size());
        const auto summed = openfhe_starter::DecryptVector(
            context, keyPair.secretKey, context->EvalSum(encryptedX, x.size()), 1);

        for (std::size_t i = 0; i < x.size(); ++i) {
            ExpectNear(added[i], x[i] + y[i], 1e-4);
            ExpectNear(multiplied[i], x[i] * y[i], 1e-4);
            ExpectNear(rotated[i], x[(i + 1) % x.size()], 1e-4);
        }
        ExpectNear(summed[0], std::accumulate(x.begin(), x.end(), 0.0), 1e-4);
        std::cout << "CKKS smoke test passed\n";
        return 0;
    }
    catch (const std::exception& exception) {
        std::cerr << "test failure: " << exception.what() << '\n';
        return 1;
    }
}
