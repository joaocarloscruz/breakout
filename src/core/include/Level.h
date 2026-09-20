#pragma once
#include <string>
#include <vector>

struct Level {
    std::string name;
    std::vector<std::string> rows; // '.' = gap, '1' = normal, '2' = armored
    float ballSpeed;
};

const std::vector<Level>& campaignLevels();
