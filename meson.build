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
    'sources/main.cpp',
    'sources/maths/vector.cpp',
    'sources/maths/matrix.cpp',
    'sources/maths/quaternion.cpp',
    'sources/physics/angular_system.cpp',
    'sources/physics/linear_system.cpp',
    'sources/3D/camera.cpp',
    'sources/3D/camera_controls.cpp',
    'sources/3D/grid.cpp',
    'sources/BVH.cpp',
    'sources/cloth.cpp',
    'sources/openGL.cpp'
]

# Dependencies (using pkg-config for discovery)
glfw = dependency('glfw3', required: true, static: true)
glbinding = dependency('glbinding', required: true, static: true)

# Target executable
executable('cxx-clothes',
           sources,
           include_directories: inc_dir,
           dependencies: [glfw, glbinding],
#           cpp_args: ['-Wall', '-Wextra', '-Werror'],
           install: true
)