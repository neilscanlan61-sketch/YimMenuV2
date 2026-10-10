From repository root:

```sh
g++ -std=c++17 -Wall -Wextra -Werror -Itests/teleport-rework -Itests/teleport-rework/mock-include -Isrc tests/teleport-rework/main.cpp -o /tmp/teleport-rework-test
/tmp/teleport-rework-test
```

Compiles the real Teleport.cpp against a fake frontend, saved-location store and game entity. This validates UI routing and value captures, not actual GTA teleport/PV behavior or real JSON disk persistence.
