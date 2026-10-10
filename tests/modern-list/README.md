# Isolated list UI and theme tests

Run from repository root (GCC, C++17):

```sh
g++ -std=c++17 -Wall -Wextra -Werror -Itests/modern-list -Itests/modern-list/mock-include tests/modern-list/main.cpp -o /tmp/yim-list-test
/tmp/yim-list-test
g++ -std=c++17 -Wall -Wextra -Werror -Isrc -Itests/modern-list -Itests/modern-list/mock-include tests/modern-list/theme-input.cpp -o /tmp/yim-theme-test
/tmp/yim-theme-test /tmp/yim-test-themes
cp -a /tmp/yim-test-themes /tmp/yim-test-themes-restart
/tmp/yim-theme-test /tmp/yim-test-themes-restart
```

Use fresh directories on each test run. Tests compile the actual renderer, editor and theme storage source against mock ImGui/frontend types. They also test the native control constants and key mapping. They do NOT simulate GTA, Windows window messages, actual ImGui behavior or a Windows DLL build.
