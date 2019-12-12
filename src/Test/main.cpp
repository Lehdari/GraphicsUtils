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

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>


using namespace gut;


void handleEvents(SDL_Event& event, App::Context& appContext)
{
    // Handle SDL events
    switch (event.type) {
        case SDL_QUIT:
            *appContext.quit = true;
            break;

        case SDL_KEYDOWN:
            // Skip events if imgui widgets are being modified
            if (ImGui::IsAnyItemActive())
                return;
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    *appContext.quit = true;
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}

void render(RenderContext& renderContext, App::Context& appContext)
{
    // Render geometry
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& mesh : renderContext.meshes) {
        mesh.render(renderContext.shader, renderContext.camera, Mat4f::Identity());
    }
}

int main(int argv, char** args)
{
    // Setup app and render context
    App::Settings appSettings;
    appSettings.handleEvents = &handleEvents;
    appSettings.render = &render;
    RenderContext context;
    App app(appSettings);
    app.setRenderContext(&context);

    // Load shader
    context.shader.load(std::string(RES_PATH) + "shaders/VS_Simple.glsl",
        std::string(RES_PATH) + "shaders/FS_Simple.glsl");
    context.shader.addUniform("objectToWorld");
    context.shader.addUniform("normalToWorld");
    context.shader.addUniform("worldToClip");
    context.shader.addUniform("Color");

    // Load mesh
    Mesh bunny;
    bunny.loadFromObj(std::string(RES_PATH) + "models/bunny.obj");
    context.meshes.push_back(std::move(bunny));

    // Enter application loop
    app.loop();

    return 0;
}
