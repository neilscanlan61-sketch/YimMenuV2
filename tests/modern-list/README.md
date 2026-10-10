# Isolated Modern List mock tests

These compile the real list renderer and pure GUI input-policy header against fake ImGui/frontend types. They do not verify the real ImGui ABI, Windows build, GTA input, or in-game behavior.

From the repository root on a machine with GCC:

```sh
g++ -std=c++17 -Itests/modern-list -Itests/modern-list/mock-include tests/modern-list/main.cpp -o /tmp/yim-list-test
/tmp/yim-list-test
```

The test source is outside the project's `src` tree and is not included in the DLL build.
