from conan import ConanFile
from conan.tools.files import copy

class FloahConan(ConanFile):
    ############################################################################
    ## Package info.                                                          ##
    ############################################################################
    
    name = "floah"
    
    description = "..."
    
    url = "https://github.com/TimZoet/Floah"

    ############################################################################
    ## Settings.                                                              ##
    ############################################################################

    python_requires = "pyreq/1.0.0@timzoet/stable"
    
    python_requires_extend = "pyreq.BaseConan"
    
    options = {
        
    }
    
    default_options = {
        
    }
    
    ############################################################################
    ## Base methods.                                                          ##
    ############################################################################
    
    def set_version(self):
        base = self.python_requires["pyreq"].module.BaseConan
        base.set_version(self, "modules/floah-layout/floahVersionString.cmake", "FLOAH_VERSION")
    
    def init(self):
        base = self.python_requires["pyreq"].module.BaseConan
        self.generators = base.generators + self.generators
        self.settings = base.settings + self.settings
        self.options = {**base.options, **self.options}
        self.default_options = {**base.default_options, **self.default_options}
    
    ############################################################################
    ## Building.                                                              ##
    ############################################################################
    
    def export_sources(self):
        copy(self, "CMakeLists.txt", self.recipe_folder, self.export_sources_folder)
        copy(self, "license", self.recipe_folder, self.export_sources_folder)
        copy(self, "readme.md", self.recipe_folder, self.export_sources_folder)
        copy(self, "cmake/*", self.recipe_folder, self.export_sources_folder)
        copy(self, "modules/*", self.recipe_folder, self.export_sources_folder)
    
    def config_options(self):
        base = self.python_requires["pyreq"].module.BaseConan
        if self.settings.os == "Windows":
            del self.options.fPIC
    
    def requirements(self):
        base = self.python_requires["pyreq"].module.BaseConan
        base.requirements(self)
        
        self.requires("common/1.0.0@timzoet/stable")
        self.requires("dot/1.0.0@timzoet/stable")
        self.requires("freetype/2.11.1")
        self.requires("icu/71.1")
        self.requires("math/1.0.0@timzoet/stable")
        self.requires("sol/1.0.0@timzoet/stable")
        self.requires("stduuid/1.0.0@timzoet/stable")
        
        if self.options.build_tests:
            self.requires("bettertest/1.0.0@timzoet/stable")
    
    def package_info(self):
        self.cpp_info.components["common"].libs = ["floah-common"]
        self.cpp_info.components["common"].requires = ["common::common", "dot::dot", "stduuid::stduuid"]
        
        self.cpp_info.components["data"].libs = ["floah-data"]
        self.cpp_info.components["data"].requires = ["common"]
        
        self.cpp_info.components["layout"].libs = ["floah-layout"]
        self.cpp_info.components["layout"].requires = ["common", "common::common", "dot::dot", "stduuid::stduuid"]
        
        self.cpp_info.components["put"].libs = ["floah-put"]
        self.cpp_info.components["put"].requires = ["common"]
        
        self.cpp_info.components["viz"].libs = ["floah-viz"]
        self.cpp_info.components["viz"].requires = ["common", "freetype::freetype", "icu::icu", "math::math", "sol::core", "sol::luna"]
        
        self.cpp_info.components["widget"].libs = ["floah-widget"]
        self.cpp_info.components["widget"].requires = ["common", "data", "layout", "put", "viz"]
    
    def generate(self):
        base = self.python_requires["pyreq"].module.BaseConan
        
        tc = base.generate_toolchain(self)
        tc.generate()
        
        deps = base.generate_deps(self)
        deps.generate()
    
    def configure_cmake(self):
        base = self.python_requires["pyreq"].module.BaseConan
        cmake = base.configure_cmake(self)
        return cmake

    def build(self):
        cmake = self.configure_cmake()
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = self.configure_cmake()
        cmake.install()
