//
// Created by Marc Seibert on 2019-02-09.
//

#ifndef LAPLACEMESHANIMATOR_RENDERER_H
#define LAPLACEMESHANIMATOR_RENDERER_H

#include "RenderObject.h"
#include "CommonStructs.h"

class Renderer {
public:
    Renderer() {};

    void Draw(Rect viewport, Camera &camera, RenderObject &object) {
        
        camera.SetViewport(viewport.position.x, viewport.position.y, viewport.width, viewport.height);
        object.Draw(camera);
    };

private:
};
#endif //LAPLACEMESHANIMATOR_RENDERER_H
