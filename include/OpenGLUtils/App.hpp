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
#include "Camera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"


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

        struct CameraSettings {
            Vec3f pos;
            Vec3f target;
            Vec3f up;
            float fov;
            float near;
            float far;

            explicit CameraSettings(
                const Vec3f& pos = Vec3f(0.f, 2.0f, 5.0f),
                const Vec3f& target = Vec3f(0.f, 1.f, 0.f),
                const Vec3f& up = Vec3f(0.f, 1.f, 0.f),
                float fov = 60.f*PI/180.f,
                float near = 0.1f,
                float far = 100.f) :
                pos(pos),
                target(target),
                up(up),
                fov(fov),
                near(near),
                far(far)
            {}
        };

        struct Settings {
            WindowSettings window;
            GLSettings gl;
            CameraSettings camera;

            explicit Settings(
                const WindowSettings& window = WindowSettings(),
                const GLSettings& gl = GLSettings(),
                const CameraSettings& camera = CameraSettings()) :
                window(window),
                gl(gl),
                camera(camera)
            {}
        };

        explicit App(const Settings& settings = Settings());

        ~App();

        void loop(void);

        void addMesh(Mesh&& mesh);

    private:
        Settings _settings;
        SDL_Window *_window;
        SDL_GLContext _glCtx;
        bool _quit; // flag for quitting the application
        uint32_t _lastTicks;
        uint32_t _frameTicks;

        Shader _shader;
        Camera _camera;
        Vector<Mesh> _meshes;

        // Window event handling loop
        void handleEvents(SDL_Event& event);

        void render(void);
    };

} // namespace gut


#endif //GRAPHICSUTILS_APP_HPP
