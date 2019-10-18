#include "png-write.h"

#include <cstdio>
#include <list>
#include <stdexcept>

#include <png.h>

namespace renderer {
    void writePng(std::filesystem::path const& img,
                  std::vector<std::vector<Colour>> const& pixels) {
        auto const height = pixels.size();
        auto const width = pixels[0].size();

        for (auto const& v : pixels) {
            if (v.size() != width) {
                throw std::runtime_error{"Different number of pixels in each row"};
            }
        }

        auto* png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

        if (!static_cast<bool>(png)) {
            throw std::runtime_error{"Failed to create PNG struct"};
        }

        auto* info = png_create_info_struct(png);

        if (!static_cast<bool>(info)) {
            png_destroy_write_struct(&png, nullptr);
            throw std::runtime_error{"Failed to create PNG info struct"};
        }

        auto const file =
            std::unique_ptr<FILE, decltype(fclose)*>{std::fopen(img.c_str(), "wb"), fclose};

        png_init_io(png, file.get());

        png_set_IHDR(png,
                     info,
                     width,
                     height,
                     8,
                     PNG_COLOR_TYPE_RGB,
                     PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_DEFAULT,
                     PNG_FILTER_TYPE_DEFAULT);

        png_write_info(png, info);

        auto rowArray = std::vector<std::uint8_t>(width * 3);
        for (auto const& row : pixels) {
            for (auto x = 0U; x < width; x++) {
                for (auto c = 0U; c < 3; c++) {
                    rowArray[x * 3 + c] = row[x][c];
                }
            }
            png_write_row(png, rowArray.data());
        }

        png_write_end(png, info);
        png_destroy_write_struct(&png, &info);
    }
} // namespace renderer
