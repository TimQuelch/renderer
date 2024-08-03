from conan import ConanFile, tools
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout


class rendererConan(ConanFile):
    name = "renderer"
    version = "0.1"
    license = "MIT"
    url = "https://github.com/TimQuelch/renderer"
    exports_sources = ("*", "!build")
    requires = ("fmt/5.3.0",
                "libpng/1.6.37",
                "eigen/3.4.0",
                "nlohmann_json/3.11.3",
                "ctre/3.9.0",
                "catch2/3.6.0")
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

