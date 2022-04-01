from conans import ConanFile

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
        base.set_version(self, "modules/floah/floahVersionString.cmake", "FLOAH_VERSION")
    
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
        self.copy("CMakeLists.txt")
        self.copy("license")
        self.copy("readme.md")
        self.copy("cmake/*")
        self.copy("modules/CMakeLists.txt")
        self.copy("modules/*")
    
    def config_options(self):
        base = self.python_requires["pyreq"].module.BaseConan
        if self.settings.os == "Windows":
            del self.options.fPIC
    
    def requirements(self):
        base = self.python_requires["pyreq"].module.BaseConan
        base.requirements(self)
        
        self.requires("common/1.0.0@timzoet/stable")
        self.requires("dot/1.0.0@timzoet/stable")
        
        if self.options.build_tests:
            self.requires("bettertest/1.0.0@timzoet/stable")
    
    def package_info(self):
        self.cpp_info.components["core"].libs = ["floah"]
        self.cpp_info.components["core"].requires = ["common::common", "dot::dot"]
    
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