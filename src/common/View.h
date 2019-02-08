//
// Created by Marc Seibert on 2019-02-08.
//

#ifndef LAPLACEMESHANIMATOR_VIEW_H
#define LAPLACEMESHANIMATOR_VIEW_H
#include "CommonStructs.h"

class View {
public:
    View() {};

    Rect GetBounds() { return bounds; };

private:
    Rect bounds;
};


#endif //LAPLACEMESHANIMATOR_VIEW_H
