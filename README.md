# renderer

Very rough usage instructions

1. Obtain and configure Conan2. Either follow instructions [here](https://docs.conan.io/2/installation.html) or use system package manager.
2. Configure your conan build profile. You can auto detect one with `conan profile detect`
3. Create build dir. `mkdir build && cd build` 
4. Install deps. `conan install .. --build missing`
5. Build `conan build ..`. Alternatively `cd Release` then `make` or `cmake --build .`
6. Run with `./renderer-app`. Configure options by editing `render-config.json`


