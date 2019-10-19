#pragma once

#include <memory>
#include <vector>

#include "colour.h"

namespace renderer {
    class Frame {
        std::vector<Colour> frame_;
        int width_;
        int height_;

    public:
        Frame(int width, int height);

        [[nodiscard]] auto width() const noexcept { return width_; }
        [[nodiscard]] auto height() const noexcept { return height_; }

        [[nodiscard]] auto at(int x, int y) noexcept -> Colour& { return frame_[y * width_ + x]; }
        [[nodiscard]] auto at(int x, int y) const noexcept -> Colour const& {
            return frame_[y * width_ + x];
        }

        [[nodiscard]] auto asPixelBuffer() const -> std::vector<std::uint8_t>;
        [[nodiscard]] auto asRowBuffer(int rowIndex) const -> std::vector<std::uint8_t>;
    };
} // namespace renderer
