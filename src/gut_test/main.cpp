//
// Project: GraphicsUtils
// File: main.cpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "gut_opengl/App.hpp"
#include "gut_opengl/RenderContext.hpp"
#include "gut_utils/VertexData.hpp"
#include "gut_utils/LoadMesh.hpp"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>


using namespace gut;

// Alias for the app type
using A_App = App<RenderContext>;

// Pipeline function for the event handling
void handleEvents(SDL_Event& event, A_App::Context& appContext)
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

// Pipeline function for rendering
void render(RenderContext& renderContext, A_App::Context& appContext)
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
    A_App::Settings appSettings;
    appSettings.handleEvents = &handleEvents;
    appSettings.render = &render;
    RenderContext context;
    A_App app(appSettings);
    app.setRenderContext(&context);

    // Load shader
    context.shader.load(std::string(RES_PATH) + "shaders/VS_Simple.glsl",
        std::string(RES_PATH) + "shaders/FS_Simple.glsl");
    context.shader.addUniform("objectToWorld");
    context.shader.addUniform("normalToWorld");
    context.shader.addUniform("worldToClip");

    // Load mesh
    VertexData bunnyData;
    loadMeshFromOBJ(std::string(RES_PATH) + "models/bunny.obj", bunnyData);
    Mesh bunny;
    bunny.loadFromVertexData(bunnyData);
    context.meshes.push_back(std::move(bunny));

    // Enter application loop
    app.loop();

    return 0;
}
