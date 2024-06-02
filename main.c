#include <raylib.h>
#include <raymath.h>
#include <sys/time.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 800

int count = 30;

void bézier(Vector2 start, Vector2 end, Vector2 mid, Color col)
{

    Vector2 prev = start;
    for (float i = 0; i < count; ++i) {
        float lerp = (float) i / count;

        Vector2 A = Vector2Lerp(start, mid, lerp);
        Vector2 B = Vector2Lerp(mid, end, lerp);

        Vector2 l = Vector2Lerp(A, B, lerp);

        DrawLineEx(prev, l, 3, RAYWHITE);
        prev = l;
    }
    DrawLineEx(prev, end, 3, RAYWHITE);
}

int main(void)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "TickTock");

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
                    if (dist < 64) {
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
            } else if ((mov = GetMouseWheelMove())) {
                if (IsKeyDown(KEY_LEFT_SHIFT)) {
                    if (mov > 0) {
                        mids_len += 1;
                        if (mids_len > 10) mids_len = 10;
                    } else {
                        mids_len -= 1;
                        if (mids_len < 1) mids_len = 1;
                    }
                } else {
                    if (mov > 0) {
                        count += 1;
                    } else {
                        count -= 1;
                        if (count < 2) count = 2;
                    }
                }
            }

            char buf[256];
            snprintf(buf, 256, "Count:    %d", count);
            DrawText(buf, WIDTH * .01, HEIGHT * .01, 24, RAYWHITE);
            snprintf(buf, 256, "mids_len: %ld", mids_len);
            DrawText(buf, WIDTH * .01, HEIGHT * .01 + 30, 24, RAYWHITE);

            ClearBackground(BLACK);

            DrawCircleV(point1, 8, RED);
            DrawCircleV(point2, 8, RED);

            for (int i = 0; i < mids_len; ++i) {
                DrawCircleV(mids[i],   8, DARKGREEN);
            }

            Vector2 prev = point1;
            for (int i = 0; i < mids_len - 1; ++i) {
                Vector2 point = Vector2Lerp(mids[i], mids[i + 1], .5);
                bézier(prev, point, mids[i], RAYWHITE);
                prev = point;
            }
            bézier(prev, point2, mids[mids_len - 1], RAYWHITE);

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
