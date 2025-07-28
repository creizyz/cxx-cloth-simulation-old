project('cxx-clothes', 'cpp',
        version: '1.0',
        default_options: ['cpp_std=c++17', 'buildtype=debugoptimized'],
        meson_version : '>=1.0.0'
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
    'sources/3D/shader.cpp',
    'sources/3D/graphics.cpp',
    'sources/3D/openGL.cpp',
    'sources/tools/toolbox.cpp',
    'sources/tools/config.cpp',
    'sources/BVH.cpp',
    'sources/cloth.cpp',
]

# Dependencies (using pkg-config for discovery)
glfw = dependency('glfw3', required: true, static: true)
glbinding = dependency('glbinding', required: true, static: true)

# Target executable
exe = executable('cxx-clothes',
           sources,
           include_directories: inc_dir,
           dependencies: [glfw, glbinding],
#           cpp_args: ['-Wall', '-Wextra', '-Werror'],
           install: true
)

# Define a custom script to copy the `assets` folder
post_install_script = find_program('cp')

custom_target(
    'copy-assets',                 # Name of the target
    input: files(),                # No specific input files
    output: ['assets.txt'],                    # No specific output files
    command: [post_install_script, '-r', join_paths(meson.project_source_root(), 'assets'), meson.project_build_root()],
    build_by_default: true         # Runs this target automatically
)
