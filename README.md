# openfhe-template

> **Tested with OpenFHE 1.1.2**

## Build and run

First [install OpenFHE](https://openfhe-development.readthedocs.io/en/latest/sphinx_rsts/intro/installation/installation.html).
Then, from the project directory, run:

```bash
mkdir build
cd build
cmake ..
make
ctest --output-on-failure
./ckks_starter
```

To run the bootstrapping example:

```bash
./ckks_bootstrap
```

If OpenFHE is installed in a custom location, configure with:

```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/openfhe
```

## Start a new OpenFHE project

1. Create a repository with GitHub's **Use this template** button (preferred),
   or clone this repository and replace its Git history.
2. Rename the `project(...)` and executable in `CMakeLists.txt`.
3. Replace `app/main.cpp`; move reusable application code into `include/` and
   `src/`.
4. Select CKKS parameters from the required circuit depth, precision, packing,
   and security target—not from the sample values alone.
5. Add fast deterministic tests and document the client/server trust boundary.
6. Remove the bootstrap example if the application does not require refresh.

## Repository layout

```text
.
├── app/                 # main executable
├── docs/                # design and learning notes
├── examples/            # optional or slower demonstrations
├── include/             # public C++ headers
├── scripts/             # repeatable local workflow
├── src/                 # reusable implementation
└── tests/               # fast automated checks
```

Read `docs/OPENFHE_NOTES.md` before adapting parameters and `SECURITY.md` before
describing any example as secure. In particular, the bootstrap example uses a
small `HEStd_NotSet` profile only to make the operation practical on a laptop.

## License

MIT. See `LICENSE`.
