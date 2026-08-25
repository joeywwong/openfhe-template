# OpenFHE starter notes

## Mental model

CKKS stores approximate real or complex values in ciphertext slots. One packed
ciphertext can therefore process a vector with SIMD-style operations. Addition
is inexpensive; multiplication consumes part of the modulus chain; rotations
and multiplication require evaluation keys; only the secret-key holder can
decrypt.

The usual application boundary is:

1. The client creates the context and keys, encodes data, and encrypts it.
2. A compute service receives ciphertexts plus only the evaluation keys needed
   by the approved circuit.
3. The service returns an encrypted result.
4. The client decrypts and interprets the approximate values.

This starter puts both roles in one process for clarity. A real system must add
serialization, separate processes or services, authenticated transport, key
storage, and a precise threat model.

## Starting a new project

This repository's `CMakeLists.txt` follows OpenFHE 1.1.2's
`CMakeLists.User.txt` example. Rename the CMake project and executable, then
replace the arithmetic in `app/main.cpp`. Put reusable code in `include/` and
`src/`, examples in `examples/`, and deterministic fast checks in `tests/`.

Choose the multiplicative depth from the circuit rather than guessing. Count
sequential ciphertext multiplications, leave an explicit margin, and inspect
levels during development. Polynomial approximations need a documented input
interval and measured approximation error.

Bootstrapping is optional. It refreshes a ciphertext so deeper computation can
continue, but it is comparatively expensive and requires additional setup and
evaluation keys. Use it when circuit depth genuinely requires it.
