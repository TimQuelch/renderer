#include "frame.h"

#include <stdexcept>

#include <fmt/format.h>

namespace renderer {
    Frame::Frame(int width, int height)
        : frame_(width * height)
        , width_{width}
        , height_{height} {}

    auto Frame::asPixelBuffer() const -> std::vector<std::uint8_t> {
        auto buffer = std::vector<std::uint8_t>{};
        buffer.reserve(width_ * height_ * 3);

        for (auto const& colour : frame_) {
            auto const pix = toPixel(colour);
            buffer.emplace_back(pix[0]);
            buffer.emplace_back(pix[1]);
            buffer.emplace_back(pix[2]);
        }

        return buffer;
    }

    auto Frame::asRowBuffer(int rowIndex) const -> std::vector<std::uint8_t> {
        if (rowIndex < 0 || rowIndex >= height_) {
            throw std::invalid_argument{
                fmt::format("Invalid row index {}. Frame height is {}", rowIndex, height_)};
        }

        auto buffer = std::vector<std::uint8_t>{};
        buffer.reserve(width_ * 3);

        for (int x = 0; x < width_; x++) {
            auto const pix = toPixel(at(x, rowIndex));
            buffer.emplace_back(pix[0]);
            buffer.emplace_back(pix[1]);
            buffer.emplace_back(pix[2]);
        }

        return buffer;
    }
} // namespace renderer
