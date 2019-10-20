#include "colour.h"

#include <limits>

namespace renderer {
    [[nodiscard]] auto toPixel(Colour const& colour) -> Pixel {
        return (colour.cwiseMin(1) * std::numeric_limits<std::uint8_t>::max()).cast<std::uint8_t>();
    }
} // namespace renderer
