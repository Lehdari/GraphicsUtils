//
// Created by Lehdari on 25.5.2019.
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
