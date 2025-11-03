#pragma once
#include "engine.hxx"
#include <swapchain.hxx>
#include <device.hxx>
#include <vector>

namespace vec{
    class Renderer{
    public:
        Renderer(const GPUDevice& rDevice, const Swapchain& rSwapchain, Engine* pEngine);
        ~Renderer();

        vk::Bool32 render();

    private:
        void createSynchronizer();
        void createCommandPool();


        const GPUDevice& rDevice;
        const Swapchain& rSwapchain;
        Engine* pEngine;

        std::vector<vk::Fence> fences;
        std::vector<vk::Semaphore> imageSignal;
        std::vector<vk::Semaphore> renderSignal;
        vk::CommandPool commandPool;
        std::vector<vk::CommandBuffer> commandBuffers;

        uint32_t imageIndex = 0;
        uint32_t currentFrame = 0;


    };
}
