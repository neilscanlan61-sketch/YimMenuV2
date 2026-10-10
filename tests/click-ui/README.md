# Click UI mock regression tests

From repository root:

```sh
g++ -std=c++17 -Wall -Wextra -Werror -Itests/click-ui -Itests/click-ui/mock-include tests/click-ui/main.cpp -o /tmp/yim-click-test
/tmp/yim-click-test /tmp/yim-click-test-themes
```

Use a fresh theme directory for each run. This compiles the real new renderer and storage/editor code against mock types. Native command widgets, Windows input delivery and actual GTA integration are not tested by this harness. Original list tests remain under tests/modern-list.
