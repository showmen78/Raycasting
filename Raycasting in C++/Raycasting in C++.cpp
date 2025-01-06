#include "raylib.h"
#include <raymath.h>
#include <vector>
#include <cmath>

// A structure to represent a 2D point
struct Point {
    float x, y;
};

// A structure to represent a line segment (an obstacle)
struct Line {
    Point start, end;
};

// Function to calculate intersection point of two lines (if it exists)
bool GetIntersection(Point p1, Point p2, Point p3, Point p4, Point& intersection) {
    float denom = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
    if (denom == 0) return false; // Lines are parallel or coincident

    float t = ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / denom;
    float u = -((p1.x - p2.x) * (p1.y - p3.y) - (p1.y - p2.y) * (p1.x - p3.x)) / denom;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        intersection.x = p1.x + t * (p2.x - p1.x);
        intersection.y = p1.y + t * (p2.y - p1.y);
        return true;
    }
    return false;
}

int main() {
    // Initialize the Raylib window
    InitWindow(800, 600, "Raycasting Simulation");
    SetTargetFPS(60);

    // Define obstacles as line segments
    std::vector<Line> obstacles = {
        {{100, 100}, {200, 150}},
        {{300, 200}, {400, 400}},
        {{600, 100}, {700, 300}},
        {{150, 450}, {650, 500}}
    };

    // Main game loop
    while (!WindowShouldClose()) {
        // Get the current mouse position (light source position)
        Point lightSource = { static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY()) };

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw obstacles
        for (const auto& line : obstacles) {
            DrawLine(line.start.x, line.start.y, line.end.x, line.end.y, WHITE);
        }

        // Cast rays in all directions
        for (float angle = 0; angle < 360; angle += 1) {
            float rayDirX = cosf(angle * DEG2RAD);
            float rayDirY = sinf(angle * DEG2RAD);
            Point rayEnd = { lightSource.x + rayDirX * 1000, lightSource.y + rayDirY * 1000 }; // Extend ray far enough

            Point closestIntersection = { rayEnd.x, rayEnd.y };
            bool intersectionFound = false;

            for (const auto& line : obstacles) {
                Point intersection;
                if (GetIntersection(lightSource, rayEnd, line.start, line.end, intersection)) {
                    if (!intersectionFound ||
                        Vector2Distance({ lightSource.x, lightSource.y }, { intersection.x, intersection.y }) <
                        Vector2Distance({ lightSource.x, lightSource.y }, { closestIntersection.x, closestIntersection.y })) {
                        closestIntersection = intersection;
                        intersectionFound = true;
                    }
                }
            }

            // Draw the ray to the closest intersection point
            DrawLine(lightSource.x, lightSource.y, closestIntersection.x, closestIntersection.y, YELLOW);
        }

        // Draw the light source
        DrawCircleV({ lightSource.x, lightSource.y }, 5, RED);

        EndDrawing();
    }

    CloseWindow(); // Close the Raylib window
    return 0;
}
