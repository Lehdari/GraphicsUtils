//
// Project: GraphicsUtils
// File: main.cpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "OpenGLUtils/App.hpp"


using namespace gut;


int main(int argv, char** args)
{
    App app;

    Mesh bunny;
    bunny.loadFromObj(std::string(RES_PATH) + "models/bunny.obj");
    app.addMesh(std::move(bunny));
    app.loop();

    return 0;
}
