//
// Project: GraphicsUtils
// File: test_opengl.cpp
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "tests.hpp"

#include <gut_opengl/App.hpp>
#include <gut_opengl/RenderContext.hpp>
#include <gut_opengl/Texture.hpp>
#include <gut_utils/VertexData.hpp>
#include <gut_utils/LoadMesh.hpp>

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
    static Vec3f cameraPos(0.0f, 1.0f, 5.0f);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render imgui
    {
        ImGui::Begin("Camera");
        ImGui::SliderFloat("X", &cameraPos(0), -10.f, 10.f);
        ImGui::SliderFloat("Y", &cameraPos(1), -10.f, 10.f);
        ImGui::SliderFloat("Z", &cameraPos(2), -10.f, 10.f);
        ImGui::End();
    }

    // Update camera
    renderContext.camera.lookAt(cameraPos, Vec3f(0.0f, 0.5f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));

    // Render geometry
    for (auto& mesh : renderContext.meshes) {
        mesh.render(renderContext.shader, renderContext.camera, Mat4f::Identity());
    }
}

int gut::testOpenGL()
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

    // Load mesh
    VertexData meshData;
    loadMeshFromOBJ(std::string(RES_PATH) + "models/box/box.obj", meshData);
    Mesh mesh;
    mesh.loadFromVertexData(meshData);
    context.meshes.push_back(std::move(mesh));

    // Load texture
    Texture texture;
    texture.loadFromFile(std::string(RES_PATH) + "models/box/box.png");
    texture.bind();

    // Enter application loop
    app.loop();

    return 0;
}
