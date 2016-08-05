/*
 * Test2_OpenGL.cpp
 *
 * This file is part of the "LLGL" project (Copyright (c) 2015 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <LLGL/LLGL.h>
#include <Gauss/Gauss.h>
#include <memory>
#include <iostream>
#include <string>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")
#endif


int main()
{
    try
    {
        // Load render system module
        auto renderer = LLGL::RenderSystem::Load("OpenGL");

        // Create render context
        LLGL::RenderContextDescriptor contextDesc;

        contextDesc.videoMode.resolution    = { 800, 600 };
        //contextDesc.videoMode.fullscreen    = true;

        contextDesc.antiAliasing.enabled    = true;
        contextDesc.antiAliasing.samples    = 8;

        contextDesc.vsync.enabled           = true;

        auto context = renderer->CreateRenderContext(contextDesc);

        context->SetClearColor(LLGL::ColorRGBAf(0.3f, 0.3f, 1));

        // Show renderer info
        auto info = context->QueryRendererInfo();

        std::cout << "Renderer:         " << info[LLGL::RendererInfo::Version] << std::endl;
        std::cout << "Vendor:           " << info[LLGL::RendererInfo::Vendor] << std::endl;
        std::cout << "Hardware:         " << info[LLGL::RendererInfo::Hardware] << std::endl;
        std::cout << "Shading Language: " << info[LLGL::RendererInfo::ShadingLanguageVersion] << std::endl;

        // Setup window title
        auto& window = context->GetWindow();

        auto title = "LLGL Test 2 ( " + renderer->GetName() + " )";
        window.SetTitle(std::wstring(title.begin(), title.end()));

        // Setup input controller
        auto input = std::make_shared<LLGL::Input>();
        window.AddEventListener(input);

        // Create vertex buffer
        auto& vertexBuffer = *renderer->CreateVertexBuffer();

        LLGL::VertexFormat vertexFormat;
        vertexFormat.AddAttribute("position", LLGL::DataType::Float, 2);

        const Gs::Vector2f vertices[] = { { -1, 1 }, { 0, 1 }, { 0, -1 }, { -1, -1 } };
        renderer->WriteVertexBuffer(vertexBuffer, vertices, sizeof(vertices), LLGL::BufferUsage::Static, vertexFormat);

        // Create vertex shader
        auto& vertShader = *renderer->CreateVertexShader();

        std::string shaderSource =
        (
            "#version 400\n"
            "layout(location=0) in vec2 position;\n"
            "out vec2 vertexPos;\n"
            "uniform Matrices {\n"
            "    mat4 projection;\n"
            "} matrices;\n"
            "void main() {\n"
            "    gl_Position = matrices.projection * vec4(position, 0.0, 1.0);\n"
            "    vertexPos = (position + vec2(1, 1))*vec2(1, 0.5);\n"
            "}\n"
        );

        if (!vertShader.Compile(shaderSource))
            std::cerr << vertShader.QueryInfoLog() << std::endl;

        // Create fragment shader
        auto& fragShader = *renderer->CreateFragmentShader();

        shaderSource =
        (
            "#version 400\n"
            "layout(location=0) out vec4 fragColor;\n"
            "in vec2 vertexPos;\n"
            "void main() {\n"
            "    fragColor = vec4(vertexPos, 0, 1);\n"
            "}\n"
        );

        if (!fragShader.Compile(shaderSource))
            std::cerr << fragShader.QueryInfoLog() << std::endl;

        // Create shader program
        auto& shaderProgram = *renderer->CreateShaderProgram();

        shaderProgram.AttachShader(vertShader);
        shaderProgram.AttachShader(fragShader);

        if (!shaderProgram.LinkShaders())
            std::cerr << shaderProgram.QueryInfoLog() << std::endl;

        shaderProgram.BindVertexAttributes(vertexFormat.GetAttributes());

        // Create constant buffer
        shaderProgram.BindAllConstantBuffers();

        // Main loop
        while (window.ProcessEvents() && !input->KeyPressed(LLGL::Key::Escape))
        {
            context->ClearBuffers(LLGL::ClearBuffersFlags::Color);

            context->SetDrawMode(LLGL::DrawMode::TriangleFan);
            context->BindShaderProgram(shaderProgram);
            context->BindVertexBuffer(vertexBuffer);

            context->Draw(4, 0);

            context->UnbindShaderProgram();

            #if defined(_WIN32) && 0
            
            auto proj = Gs::ProjectionMatrix4f::Planar(
                static_cast<Gs::Real>(contextDesc.videoMode.resolution.x),
                static_cast<Gs::Real>(contextDesc.videoMode.resolution.y)
            );

            glMatrixMode(GL_PROJECTION);
            glLoadMatrixf(proj.Ptr());

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glBegin(GL_LINES);
            {
                glVertex2i(200, 100);
                glVertex2i(400, 200);
            }
            glEnd();

            #endif

            context->Present();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        #ifdef _WIN32
        system("pause");
        #endif
    }

    return 0;
}