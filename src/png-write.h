#pragma once

#include <filesystem>

namespace renderer {
    class Frame;

    void writePng(std::filesystem::path const& img, Frame const& frame);
} // namespace renderer
