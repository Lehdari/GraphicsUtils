//
// Project: GraphicsUtils
// File: App.cpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "App.hpp"

#include <GL/gl3w.h>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <cmath>
#include <functional>


using namespace gut;


App::App(const App::Settings &settings,
    RenderContext* renderContext
    ) :
    _settings       (settings),
    _window         (nullptr),
    _quit           (false),
    _lastTicks      (0),
    _frameTicks     (0),
    _renderContext  (renderContext)
{
    int err;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error: Could not initialize SDL!\n");
        return;
    }

    _window = SDL_CreateWindow(
        _settings.window.name.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        (int)_settings.window.width,
        (int)_settings.window.height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (_window == nullptr) {
        printf("Error: SDL Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, _settings.gl.contextMajor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, _settings.gl.contextMinor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, _settings.gl.contextFlags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, _settings.gl.profileMask);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, _settings.gl.doubleBuffer);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    _glCtx = SDL_GL_CreateContext(_window);

    if (_glCtx == nullptr) {
        printf("Error: SDL OpenGL context could not be created! SDL_Error: %s\n",
            SDL_GetError());
        return;
    }

    // Load OpenGL binds
    err = gl3wInit();
    if (err) {
        printf("Error: gl3wInit failed\n");
        return;
    }

    // Initialize imgui
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(_window, _glCtx);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Initialize OpenGL
    glViewport(0, 0, _settings.window.width, _settings.window.height);
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glEnable(GL_DEPTH_TEST);
}

App::~App()
{
    // Destroy imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // Destroy window and quit SDL subsystems
    SDL_GL_DeleteContext(_glCtx);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void App::loop(void)
{
    // Application main loop
    while (!_quit) {
        // Event handling
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (_settings.handleEvents != nullptr)
                _settings.handleEvents(event, _quit);
        }

        // Initialize imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(_window);
        ImGui::NewFrame();

        // Generate draw data
        ImGui::Render();

        // User-defined render
        if (_renderContext != nullptr && _settings.render != nullptr)
            _settings.render(*_renderContext);

        // Render imgui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap draw and display buffers
        SDL_GL_SwapWindow(_window);

        uint32_t curTicks = SDL_GetTicks();
        _frameTicks = curTicks - _lastTicks;
        _lastTicks = curTicks;
    }
}

void App::setRenderContext(RenderContext* context)
{
    _renderContext = context;
}
