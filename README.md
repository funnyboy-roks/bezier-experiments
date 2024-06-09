# Bézier Experiments

A simple experiment with making Bézier curves, nothing special, and
certainly not the most efficient.

![Screenshot of program running](./img/screenshot.png)

## Controls

| Control              | Action                                                |
| -------------------- | ----------------------------------------------------- |
| Left Click + Drag    | Move point                                            |
| Scroll Up/Down       | Increase/Decrease count of steps when drawing curves  |
| Shift Scroll Up/Down | Increase/Decrease count of mid points (min 1, max 10) |
| 1, 2, 3, ..., 0      | Put a midpoint at your cursor position                |
| T                    | Toggle showing traces                                 |
| O                    | Toggle showing only the curve                         |
| R                    | Reset all points                                      |

## Building

[RayLib](https://raylib.com) will need to be installed to build this project.

```sh
make
# or just
cc -o main main.c -lraylib
```
