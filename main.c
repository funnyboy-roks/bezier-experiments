#include <raylib.h>
#include <raymath.h>
#include <string.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 800

int steps = 30;
bool show_traces = false;
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

void lerp_pts(float t, Vector2 *pts, size_t pts_len)
{
    for (size_t i = 0; i < pts_len - 1; ++i) {
        pts[i] = Vector2Lerp(pts[i], pts[i + 1], t);
    }
}

static const Color colours[] = {
    GRAY,
    YELLOW,
    ORANGE,
    PINK,
    RED,
    MAROON,
    LIME,
    SKYBLUE,
    VIOLET,
};
#define colours_len (sizeof(colours) / sizeof(colours[0]))

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
    // -1  -- no selection
    // -2  -- point1
    // -3  -- point2
    // 0.. -- mids[selected]
    int selected = -1;
    while (!WindowShouldClose()) {
        BeginDrawing();

            float mov;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                const float dist_req = 15 * 15;
                Vector2 mouse_pos = GetMousePosition();
                if (Vector2DistanceSqr(point1, mouse_pos) < dist_req) {
                    selected = -2;
                } else if (Vector2DistanceSqr(point2, mouse_pos) < dist_req) {
                    selected = -3;
                } else {
                    for (int i = 0; i < mids_len; ++i) {
                        if (Vector2DistanceSqr(mids[i], GetMousePosition()) < dist_req) {
                            selected = i;
                            break;
                        }
                    }
                }
            } else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (selected == -2) {
                    point1 = GetMousePosition();
                } else if (selected == -3) {
                    point2 = GetMousePosition();
                } else if (selected != -1) {
                    mids[selected] = GetMousePosition();
                }
            } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                selected = -1;
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
            snprintf(buf, 256, "Only show curve: %s [O]", only_curve ? "Yes" : "No");
            DrawText(buf, WIDTH * .01, HEIGHT * .01 + 30 * y++, 24, RAYWHITE);

            ClearBackground(GetColor(0x111111FF));

            Vector2 prev = point1;
            for (size_t i = 0; i <= steps; ++i) {
                float lerp = (float) i / steps;

                size_t points_len = 2 + mids_len;
                Vector2 points[points_len];
                points[0] = point1;
                for (size_t j = 0; j < mids_len; ++j) {
                    points[j + 1] = mids[j];
                }
                points[points_len - 1] = point2;

                for (size_t step_count = 0; points_len > 1; step_count++) {

                    lerp_pts(lerp, points, points_len);
                    points_len -= 1;

                    if (show_traces) {
                        for (size_t j = 0; j < points_len - 1; ++j) {
                            DrawLineEx(points[j], points[j + 1], 3, colours[step_count % colours_len]);
                        }
                    }
                }
                DrawLineEx(prev, points[0], 4, RAYWHITE);
                prev = points[0];
            }
            DrawLineEx(prev, point2, 4, RAYWHITE);

            if (!only_curve) {
                DrawLineEx(point1, mids[0], 1, GRAY);
                for(int i = 0; i < mids_len - 1; ++i) {
                    DrawLineEx(mids[i], mids[i+1], 1, GRAY);
                    DrawCircleV(Vector2Lerp(mids[i], mids[i + 1], .5), 4, GRAY);
                }
                DrawLineEx(mids[mids_len - 1], point2, 1, GRAY);

                DrawCircleV(point1, 8, RED);
                DrawCircleV(point2, 8, BLUE);

                for (int i = 0; i < mids_len; ++i) {
                    DrawCircleV(mids[i],   8, DARKGREEN);
                }
            }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
