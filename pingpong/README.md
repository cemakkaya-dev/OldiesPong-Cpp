# OldiesPong

A classic Pong game written in C++ using SDL2.

## Gameplay

- First player to reach **5 points** wins
- The ball waits 2 seconds at the center after each score, then launches in a random direction

## Controls

| Action | Left Player | Right Player |
|--------|-------------|--------------|
| Move Up | `W` | `↑` |
| Move Down | `S` | `↓` |

## Dependencies

- [SDL2](https://www.libsdl.org/)
- [SDL2_ttf](https://wiki.libsdl.org/SDL2_ttf/FrontPage)

### macOS (Homebrew)

```bash
brew install sdl2 sdl2_ttf
```

## Build & Run

```bash
make
./ping_pong
```
