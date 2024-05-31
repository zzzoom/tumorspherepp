#pragma once

struct Position {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    bool operator==(const Position& other) const
    {
        return (x == other.x) && (y == other.y) && (z == other.z);
    }
};
