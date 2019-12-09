#include "obj.h"

#include <fstream>
#include <string>
#include <string_view>

#include <ctre.hpp>

#include "scene.h"
#include "vec.h"

namespace renderer {
    namespace {
        [[nodiscard]] auto toFloat(std::string_view str) { return std::stof(std::string{str}); }
        [[nodiscard]] auto toIndex(std::string_view str, long size) {
            auto const index = std::stol(std::string{str});
            if (index < 0) {
                return index + size;
            }
            return index - 1;
        }
    } // namespace

    [[nodiscard]] auto loadObj(std::filesystem::path const& objFile,
                               Colour const& background,
                               Material const& material) -> Scene {
        auto file = std::ifstream{objFile};

        constexpr static auto tokenRE = ctll::fixed_string{R"(\s*((#.*)|(\S+)))"};
        auto line = std::string{};

        auto vertices = std::vector<Vec>{};
        auto scene = Scene{background};

        while (std::getline(file, line)) {
            auto const lineSV = std::string_view{line};
            auto tokens = std::vector<std::string_view>{};
            for (auto match : ctre::range<tokenRE>(lineSV)) {
                if (!match) {
                    break;
                }

                auto const view = match.view();
                if (view[0] != '#') {
                    tokens.emplace_back(match.view());
                }
            }

            if (tokens.empty()) {
                continue;
            }

            auto const command = tokens.front();
            tokens.erase(tokens.begin());

            using namespace std::literals;

            if (command == "v"sv) {
                if (tokens.size() < 3 || tokens.size() > 4) {
                    throw std::runtime_error{"Verticies must have 3 (or 4) parameters"};
                }
                vertices.push_back({toFloat(tokens[0]), toFloat(tokens[1]), toFloat(tokens[2])});
            } else if (command == "f"sv) {
                if (tokens.size() < 3) {
                    throw std::runtime_error{"Faces must have more than 3 parameters"};
                }
                for (auto it = tokens.cbegin() + 2; it < tokens.cend(); it++) {
                    scene.addTriangle(vertices[toIndex(*tokens.begin(), tokens.size())],
                                      vertices[toIndex(*(it - 1), tokens.size())],
                                      vertices[toIndex(*it, tokens.size())],
                                      material);
                }
            }
        }
        return scene;
    }
} // namespace renderer
