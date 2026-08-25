# Security scope

This repository is an educational and research-development starter, not a
production cryptographic system or a security audit.

The default `ckks_starter` example requests `HEStd_128_classic`, allowing
OpenFHE to select parameters from its security tables. Security still depends
on the complete application: parameter choices, key custody, serialization,
transport, access control, side channels, and the threat model.

The optional `ckks_bootstrap` example deliberately uses `HEStd_NotSet` and a
small ring dimension to run on a development machine. It demonstrates the API
but makes **no standard security claim**. Do not copy those demonstration
parameters into a deployment.

Never commit secret keys, private datasets, credentials, generated key files,
or sensitive output. The ignored `tmp/` and `output/` directories are local
conveniences, not secure storage.

