from conans import ConanFile, tools, CMake

class renderrConan(ConanFile):
    name = "renderer"
    version = "0.1"
    license = "MIT"
    url = "https://github.com/TimQuelch/renderer"
    exports_sources = ("*", "!build")
    generators = ("cmake_paths")
    build_requires = ("fmt/5.3.0@bincrafters/stable")

    def build_requirements(self):
        if tools.get_env("CONAN_RUN_TESTS", True):
            self.build_requires("Catch2/2.9.2@catchorg/stable")

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["AUDIO_ENABLE_TESTING"] = "ON" if tools.get_env("CONAN_RUN_TESTS", True) else "OFF"
        cmake.configure()
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()
        if tools.get_env("CONAN_RUN_TESTS", True):
            cmake.test()
