#pragma once

#include <filesystem>

#include "material.h"

namespace renderer {
    void writePng(std::filesystem::path const& img, std::vector<std::vector<Colour>> const& pixels);
}
