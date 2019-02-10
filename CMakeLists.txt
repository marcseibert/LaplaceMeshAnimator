cmake_minimum_required(VERSION 3.13)


project(LaplaceMeshAnimator)

set(CMAKE_CXX_STANDARD 17)

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
        src/common/Camera.cpp
        src/common/Shader.hpp
        src/common/ShaderManager.hpp
        src/common/ShaderManager.cpp
        src/common/Util.hpp
        src/common/Util.cpp
        src/common/SceneNode.h

        src/common/drawables/Sprite.hpp
        src/common/drawables/Sprite.cpp
        src/common/drawables/Box.hpp
        src/common/drawables/Box.cpp
        src/common/drawables/Mesh.h
        src/common/drawables/Mesh.cpp
        src/common/drawables/Model.hpp
        src/common/drawables/Model.cpp
        src/common/modifier/MeshModifier.hpp

        src/LaplaceAnimator.h

        src/extern/stb_image.h
        src/extern/stb_image.cpp
        src/common/RenderObject.h
        src/common/Renderer.h
        src/InspectorView.h
        src/SceneView.h
        src/common/input/MouseInput.h
        src/common/DraggableObject.h
        )

target_include_directories(${PROJECT_NAME} PRIVATE include
        /usr/local/include
        /Users/marcseibert/Libraries/assimp/include
        src/exter/assimp/include
        src/extern/eigen
        )

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME}
        /Users/marcseibert/Libraries/assimp/lib/libassimp.4.1.0.dylib # ASSIMP
        /usr/local/Cellar/glew/2.1.0/lib/libGLEW.2.1.0.dylib #GLEW
        /usr/local/Cellar/glfw/HEAD-f9923e9/lib/libglfw.3.3.dylib #GLFW
        ${OPENGL_LIBRARIES}
)
