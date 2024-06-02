#include <raylib.h>
#include <raymath.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 800

int steps = 30;
bool show_traces = false;
bool cubic = true;
bool only_curve = false;

void bézier(Vector2 start, Vector2 end, Vector2 mid, Color col)
{

    Vector2 prev = start;
    for (float i = 0; i <= steps; ++i) {
        float lerp = (float) i / steps;

        Vector2 A = Vector2Lerp(start, mid, lerp);
        Vector2 B = Vector2Lerp(mid, end, lerp);

        if (show_traces) {
            DrawLineEx(A, B, 1, YELLOW);
        }

        Vector2 l = Vector2Lerp(A, B, lerp);

        DrawLineEx(prev, l, 3, col);
        prev = l;
    }
    DrawLineEx(prev, end, 3, col);
}

int main(void)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Bézier Experiments");

    Vector2 point1 = {
        WIDTH * .25,
        HEIGHT * .25,
    };

    Vector2 point2 = {
        WIDTH * .75,
        HEIGHT * .75,
    };


    Vector2 x_axis = {
        1, 
        0,
    };

    // Vector2 p1_to_mid = Vector2Subtract(mid, point1);
    // float p1_mid_len = Vector2Length(p1_to_mid);
    // p1_to_mid = Vector2Normalize(p1_to_mid);

    // Vector2 mid_to_p2 = Vector2Subtract(point2, mid);
    // float mid_p2_len = Vector2Length(mid_to_p2);
    // mid_to_p2 = Vector2Normalize(mid_to_p2);

    Vector2 mids[10] = {
        [0] = {
            WIDTH * .5,
            HEIGHT * .25,
        },
        [1] = {
            WIDTH * .75,
            HEIGHT * .5,
        },
        0,
    };

    size_t mids_len = 2;
    int selected = -1;
    while (!WindowShouldClose()) {
        BeginDrawing();

            float mov;
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                point1 = GetMousePosition();
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
                for (int i = 0; i < mids_len; ++i) {
                    float dist = Vector2DistanceSqr(mids[i], GetMousePosition());
                    if (dist < 15 * 15) {
                        selected = i;
                        break;
                    }
                }
            } else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
                if (selected != -1) {
                    mids[selected] = GetMousePosition();
                }
            } else if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)) {
                selected = -1;
            } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                point2 = GetMousePosition();
            } else if (IsKeyDown(KEY_ONE)) {
                mids[0] = GetMousePosition();
            } else if (IsKeyDown(KEY_TWO)) {
                mids[1] = GetMousePosition();
            } else if (IsKeyDown(KEY_THREE)) {
                mids[2] = GetMousePosition();
            } else if (IsKeyDown(KEY_FOUR)) {
                mids[3] = GetMousePosition();
            } else if (IsKeyDown(KEY_FIVE)) {
                mids[4] = GetMousePosition();
            } else if (IsKeyDown(KEY_SIX)) {
                mids[5] = GetMousePosition();
            } else if (IsKeyDown(KEY_SEVEN)) {
                mids[6] = GetMousePosition();
            } else if (IsKeyDown(KEY_EIGHT)) {
                mids[7] = GetMousePosition();
            } else if (IsKeyDown(KEY_NINE)) {
                mids[8] = GetMousePosition();
            } else if (IsKeyDown(KEY_ZERO)) {
                mids[9] = GetMousePosition();
            } else if (IsKeyPressed(KEY_T)) {
                if(show_traces ^= true) {
                    only_curve = false;
                }
            } else if (IsKeyPressed(KEY_C)) {
                if (mids_len == 2) {
                    cubic ^= true;
                }
            } else if (IsKeyPressed(KEY_O)) {
                if (only_curve ^= true) {
                    show_traces = false;
                }
            } else if ((mov = GetMouseWheelMove())) {
                if (IsKeyDown(KEY_LEFT_SHIFT)) {
                    if (mov > 0) {
                        mids_len += 1;
                        if (mids_len > 10) mids_len = 10;
                        Vector2 *new_mid = &mids[mids_len - 1];
                        if (
                            new_mid->x <= 0 || new_mid->x > GetScreenWidth()
                            || new_mid->y <= 0 || new_mid->y > GetScreenHeight()
                        ) {
                            *new_mid = (Vector2) {
                                .x = GetScreenWidth() / 2.,
                                .y = GetScreenHeight() / 2.,
                            };
                        }
                    } else {
                        mids_len -= 1;
                        if (mids_len < 1) mids_len = 1;
                    }
                } else {
                    if (mov > 0) {
                        steps += 1;
                    } else {
                        steps -= 1;
                        if (steps < 2) steps = 2;
                    }
                }
            }

            char buf[256];
            int y = 0;
            snprintf(buf, 256, "# of Steps: %d", steps);
            DrawText(buf, WIDTH * .01, HEIGHT * .01 + 30 * y++, 24, RAYWHITE);
            snprintf(buf, 256, "Show Traces: %s [T]", show_traces ? "Yes" : "No");
            DrawText(buf, WIDTH * .01, HEIGHT * .01 + 30 * y++, 24, RAYWHITE);
            snprintf(buf, 256, "# of Mids: %ld", mids_len);
            DrawText(buf, WIDTH * .01, HEIGHT * .01 + 30 * y++, 24, RAYWHITE);
            if (mids_len == 2) {
                snprintf(buf, 256, "Use cubic: %s [C]", cubic ? "Yes" : "No");
                DrawText(buf, WIDTH * .01, HEIGHT * .01 + 30 * y++, 24, RAYWHITE);
            }
            snprintf(buf, 256, "Only show curve: %s [O]", only_curve ? "Yes" : "No");
            DrawText(buf, WIDTH * .01, HEIGHT * .01 + 30 * y++, 24, RAYWHITE);

            ClearBackground(GetColor(0x111111FF));

            if (mids_len == 2 && cubic) {
                Vector2 prev = point1;
                for (float i = 0; i <= steps; ++i) {
                    float lerp = (float) i / steps;

                    Vector2 A = Vector2Lerp(point1, mids[0], lerp);
                    Vector2 B = Vector2Lerp(mids[0], mids[1], lerp);
                    Vector2 C = Vector2Lerp(mids[1], point2, lerp);

                    Vector2 AB = Vector2Lerp(A, B, lerp);
                    Vector2 BC = Vector2Lerp(B, C, lerp);

                    Vector2 l = Vector2Lerp(AB, BC, lerp);

                    if (show_traces) {
                        DrawLineEx(A, B, 1, RAYWHITE);
                        DrawLineEx(B, C, 1, YELLOW);
                        DrawLineEx(AB, BC, 1, SKYBLUE);
                    }

                    DrawLineEx(prev, l, 4, RAYWHITE);
                    prev = l;
                }
                DrawLineEx(prev, point2, 4, RAYWHITE);
            } else {
                Vector2 prev = point1;
                for (int i = 0; i < mids_len - 1; ++i) {
                    Vector2 point = Vector2Lerp(mids[i], mids[i + 1], .5);
                    bézier(prev, point, mids[i], RAYWHITE);
                    prev = point;
                }
                bézier(prev, point2, mids[mids_len - 1], RAYWHITE);
            }

            if (!only_curve) {
                DrawLineEx(point1, mids[0], 1, GRAY);
                if (!cubic || mids_len != 2) {
                    for(int i = 0; i < mids_len - 1; ++i) {
                        DrawLineEx(mids[i], mids[i+1], 1, GRAY);
                        DrawCircleV(Vector2Lerp(mids[i], mids[i + 1], .5), 4, GRAY);
                    }
                }
                DrawLineEx(mids[mids_len - 1], point2, 1, GRAY);

                DrawCircleV(point1, 8, RED);
                DrawCircleV(point2, 8, BLUE);

                for (int i = 0; i < mids_len; ++i) {
                    DrawCircleV(mids[i],   8, DARKGREEN);
                }
            }

            // Vector2 true_mid = Vector2Lerp(point1, point2, 0.5);
            // if (Vector2DistanceSqr(mid, true_mid) > .01) {
            //     mid = Vector2Lerp(mid, true_mid, .0005);
            // }
            // DrawLineBezier(point1, point2, 2, YELLOW);

            // Vector2 curr = Vector2Add(point1, (Vector2) { WIDTH * .25, 0 } );

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
