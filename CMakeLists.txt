cmake_minimum_required(VERSION 3.13)


project(LaplaceMeshAnimator)

set(CMAKE_CXX_STANDARD 14)

# INCLUDE DIRS


#find_package(glew REQURIED)
#find_package(glm PATHS /)
#find_package(glfw3 3.3 REQUIRED)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/common/View.cpp
        src/common/View.h
        src/common/CommonStructs.h
        src/common/GraphicsProgram.h
        src/common/Camera.hpp
        src/common/Shader.hpp
        src/common/ShaderManager.hpp
        src/common/ShaderManager.cpp
        src/common/Util.hpp

        src/common/drawables/Drawable.hpp
        src/common/drawables/Sprite.hpp
        src/common/drawables/Sprite.cpp

        src/common/modifier/MeshModifier.hpp


        src/LaplaceAnimator.h
        src/common/Mesh.h)

target_include_directories(${PROJECT_NAME} PRIVATE include
        /usr/local/include
        /Users/marcseibert/Libraries/assimp/include
        )

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME}
        /Users/marcseibert/Libraries/assimp/lib/libassimp.4.1.0.dylib # ASSIMP
        /usr/local/Cellar/glew/2.1.0/lib/libGLEW.2.1.0.dylib #GLEW
        /usr/local/Cellar/glfw/HEAD-f9923e9/lib/libglfw.3.3.dylib #GLFW
        ${OPENGL_LIBRARIES}
)
