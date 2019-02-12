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
        src/common/Camera.h
        src/common/Camera.cpp
        src/common/Shader.h
        src/common/ShaderManager.h
        src/common/ShaderManager.cpp
        src/common/Util.h
        src/common/Util.cpp
        src/common/SceneNode.h


        src/common/drawables/Sprite.h
        src/common/drawables/Sprite.cpp
        src/common/drawables/Box.h
        src/common/drawables/Box.cpp
        src/common/drawables/Mesh.h
        src/common/drawables/Mesh.cpp
        src/common/drawables/Model.h
        src/common/drawables/Model.cpp
        src/common/modifier/MeshModifier.h
        src/common/modifier/EditableMesh.h
        src/common/modifier/EditableMesh.cpp

        src/LaplaceAnimator.h

        src/extern/stb_image.h
        src/extern/stb_image.cpp
        src/common/RenderObject.h
        src/common/Renderer.h
        src/InspectorView.h
        src/SceneView.h
        src/common/input/MouseInput.h
        src/common/DraggableObject.h
        src/common/collision/Raycaster.h
        src/common/input/MouseHook.h
        src/common/collision/IntersectionRect.h
        src/common/collision/IntersectionMesh.h
        src/common/modifier/EditableModel.h
        src/common/RenderIDObject.h
        src/common/modifier/VertexGrabModifier.h
        src/common/modifier/LaplaceMeshModifier.h
        src/common/modifier/MeshMatrix.h)

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
