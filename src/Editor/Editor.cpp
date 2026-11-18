#include <ODPpch.h>
#include "Editor.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlgpu3.h"

namespace ODP {

    Editor::~Editor()
    {
        quit();
    }

    bool Editor::init()
    {
        m_Window = SDL_CreateWindow("OpenDesktopPet", 1280, 720,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY);
        if (!m_Window)
        {
            quit();
            return false;
        }
        SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_ShowWindow(m_Window);

        m_GpuDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL | SDL_GPU_SHADERFORMAT_METALLIB, true, nullptr);
        if (!m_GpuDevice)
        {
            quit();
            return false;
        }

        if (!SDL_ClaimWindowForGPUDevice(m_GpuDevice, m_Window))
        {
            quit();
            return false;
        }
        SDL_SetGPUSwapchainParameters(m_GpuDevice, m_Window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC);

        ImGui_ImplSDL3_InitForSDLGPU(m_Window);
        ImGui_ImplSDLGPU3_InitInfo init_info = {};
        init_info.Device = m_GpuDevice;
        init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(m_GpuDevice, m_Window);
        init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;                      // Only used in multi-viewports mode.
        init_info.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;  // Only used in multi-viewports mode.
        init_info.PresentMode = SDL_GPU_PRESENTMODE_VSYNC;
        ImGui_ImplSDLGPU3_Init(&init_info);
        m_ImguiInitialized = true;

        return true;
    }

    void Editor::update()
    {
        ImGui_ImplSDLGPU3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGuiIO& io = ImGui::GetIO();

        ImGui::SetNextWindowSize(io.DisplaySize);
        int x, y;
        SDL_GetWindowPosition(m_Window, &x, &y);
        ImGui::SetNextWindowPos(ImVec2{ (float)x, (float)y });
        ImGui::Begin("OpenDesktopPet", nullptr, 
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        ImGui::End();

        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

        SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(m_GpuDevice);

        SDL_GPUTexture* swapchain_texture;
        SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, m_Window, &swapchain_texture, nullptr, nullptr);

        if (swapchain_texture != nullptr && !is_minimized)
        {
            // This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to upload the vertex/index buffer!
            ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

            // Setup and start a render pass
            SDL_GPUColorTargetInfo target_info = {};
            target_info.texture = swapchain_texture;
            target_info.clear_color = SDL_FColor{ 0.0f, 0.0f, 0.0f, 1.0f };
            target_info.load_op = SDL_GPU_LOADOP_CLEAR;
            target_info.store_op = SDL_GPU_STOREOP_STORE;
            target_info.mip_level = 0;
            target_info.layer_or_depth_plane = 0;
            target_info.cycle = false;
            SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);

            // Render ImGui
            ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);

            SDL_EndGPURenderPass(render_pass);
        }
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        SDL_SubmitGPUCommandBuffer(command_buffer);
    }

    void Editor::quit()
    {
        if (m_ImguiInitialized)
        {
            SDL_WaitForGPUIdle(m_GpuDevice);
            ImGui_ImplSDL3_Shutdown();
            ImGui_ImplSDLGPU3_Shutdown();
        }

        SDL_ReleaseWindowFromGPUDevice(m_GpuDevice, m_Window);
        SDL_DestroyGPUDevice(m_GpuDevice);
        SDL_DestroyWindow(m_Window);
    }

}