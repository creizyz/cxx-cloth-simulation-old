project('cxx-clothes', 'cpp',
        version: '1.0',
        default_options: ['cpp_std=c++17', 'buildtype=debugoptimized']
)

# Include directories
inc_dir = include_directories(
    'includes',
)

# Source files
sources = [
    'sources/Camera.cpp',
    'sources/main.cpp',
    'sources/math.cpp',
    'sources/motion_system.cpp',
    'sources/openGL.cpp'
]

# Dependencies (using pkg-config for discovery)
glfw = dependency('glfw3', required: true, static: true)
glbinding = dependency('glbinding', required: true, static: true)

# Target executable
executable('cxxclothsimulation',
           sources,
           include_directories: inc_dir,
           dependencies: [glfw, glbinding],
           cpp_args: ['-Wall', '-Wextra', '-Werror'],
           install: true
)