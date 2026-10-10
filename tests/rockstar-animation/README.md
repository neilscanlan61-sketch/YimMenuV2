# Portable animation and camera policy tests

Run from the project root:

```sh
g++ -std=c++17 -Wall -Wextra -Werror tests/rockstar-animation/main.cpp -o animation-test
./animation-test
```

Uses the real portable compositor/timing and camera-entry policy headers.
Does NOT exercise Windows WIC, COM, D3D12 uploads/descriptor lifetime, native game
calls or actual camera behavior. Click UI tests stub out the Windows Renderer interface.
