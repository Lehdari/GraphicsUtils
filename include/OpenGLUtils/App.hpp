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

#include "Utils/MathUtils.hpp"
#include "Utils/TypeUtils.hpp"
#include "RenderContext.hpp"


namespace gut {

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

        struct Settings {
            WindowSettings window;
            GLSettings gl;

            // Function pointers for event handling and rendering
            void (*handleEvents)(SDL_Event& event, bool& quit);
            void (*render)(RenderContext& context);


            explicit Settings(
                const WindowSettings& window            = WindowSettings(),
                const GLSettings& gl                    = GLSettings(),
                void (*handleEvents)(SDL_Event&, bool&) = nullptr,
                void (*render)(RenderContext&)          = nullptr
                ) :
                window          (window),
                gl              (gl),
                handleEvents    (handleEvents),
                render          (render)
            {}
        };

        explicit App(
            const Settings& settings = Settings(),
            RenderContext* renderContext = nullptr);

        ~App();

        void loop(void);

        void setRenderContext(RenderContext* context);

    private:
        Settings _settings;
        SDL_Window *_window;
        SDL_GLContext _glCtx;
        bool _quit; // flag for quitting the application
        uint32_t _lastTicks;
        uint32_t _frameTicks;

        RenderContext*  _renderContext;
    };

} // namespace gut


#endif //GRAPHICSUTILS_APP_HPP
