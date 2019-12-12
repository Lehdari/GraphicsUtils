//
// Project: GraphicsUtils
// File: App.hpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_APP_HPP
#define GRAPHICSUTILS_APP_HPP


#include <string>
#include <SDL.h>
#include <GL/gl3w.h>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include "Utils/MathUtils.hpp"
#include "Utils/TypeUtils.hpp"
#include "RenderContext.hpp"


namespace gut {

    template <typename T_RenderContext = gut::RenderContext>
    class App {
    public:
        // Settings for the application
        struct WindowSettings {
            std::string name;
            int64_t width;
            int64_t height;
            int64_t framerateLimit;

            explicit WindowSettings(
                const std::string& name = "",
                int64_t width = 1280,
                int64_t height = 720,
                int64_t framerateLimit = 60) :
                name(name),
                width(width),
                height(height),
                framerateLimit(framerateLimit)
            {}
        };

        struct GLSettings {
            int32_t contextMajor;
            int32_t contextMinor;
            SDL_GLcontextFlag contextFlags;
            SDL_GLprofile profileMask;
            bool doubleBuffer;

            explicit GLSettings(
                int32_t contextMajor = 3,
                int32_t contextMinor = 3,
                SDL_GLcontextFlag contextFlags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG,
                SDL_GLprofile profileMask = SDL_GL_CONTEXT_PROFILE_CORE,
                bool doubleBuffer = true) :
                contextMajor(contextMajor),
                contextMinor(contextMinor),
                contextFlags(contextFlags),
                profileMask(profileMask),
                doubleBuffer(doubleBuffer)
            {}
        };

        // Struct for App context to be passed to pipeline functions
        struct Context {
            WindowSettings* windowSettings;
            GLSettings*     glSettings;
            SDL_Window*     window;
            SDL_GLContext*  glContext;
            bool*           quit;

            Context(App& app) :
                windowSettings  (&app._settings.window),
                glSettings      (&app._settings.gl),
                window          (app._window),
                glContext       (&app._glCtx),
                quit            (&app._quit)
            {}
        };

        struct Settings {
            WindowSettings window;
            GLSettings gl;

            // Pipeline function pointers for event handling and rendering
            void (*handleEvents)(SDL_Event& event, Context& appContext);
            void (*render)(T_RenderContext& context, Context& appContext);

            explicit Settings(
                const WindowSettings& window                            = WindowSettings(),
                const GLSettings& gl                                    = GLSettings(),
                void (*handleEvents)(SDL_Event&, Context& appContext)   = nullptr,
                void (*render)(T_RenderContext&, Context& appContext)   = nullptr
                ) :
                window          (window),
                gl              (gl),
                handleEvents    (handleEvents),
                render          (render)
            {}
        };

        explicit App(
            const Settings& settings = Settings(),
            T_RenderContext* renderContext = nullptr);

        ~App();

        void loop(void);

        void setRenderContext(T_RenderContext* renderContext);

    private:
        Settings            _settings;
        SDL_Window*         _window;
        SDL_GLContext       _glCtx;
        bool                _quit; // flag for quitting the application
        uint32_t            _lastTicks;
        uint32_t            _frameTicks;

        App::Context        _appContext;
        T_RenderContext*    _renderContext;
    };

    #include "App.inl"

} // namespace gut


#endif //GRAPHICSUTILS_APP_HPP
