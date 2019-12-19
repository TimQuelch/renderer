from conans import ConanFile, tools, CMake

class rendererConan(ConanFile):
    name = "renderer"
    version = "0.1"
    license = "MIT"
    url = "https://github.com/TimQuelch/renderer"
    exports_sources = ("*", "!build")
    generators = ("cmake_paths")
    build_requires = ("fmt/5.3.0@bincrafters/stable",
                      "libpng/1.6.37@bincrafters/stable",
                      "eigen/3.3.7@conan/stable",
                      "jsonformoderncpp/3.7.0@vthiery/stable",
                      "CTRE/v2.7@ctre/stable")

    def build_requirements(self):
        if tools.get_env("CONAN_RUN_TESTS", True):
            self.build_requires("Catch2/2.10.0@catchorg/stable")

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["RENDERER_ENABLE_TESTING"] = "ON" if tools.get_env("CONAN_RUN_TESTS", True) else "OFF"
        cmake.configure()
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()
        if tools.get_env("CONAN_RUN_TESTS", True):
            cmake.test()
