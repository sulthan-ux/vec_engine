#pragma once
#include <swapchain.hxx>
#include <device.hxx>
#include <vector>

namespace vec{
    class Renderer{
    public:
        Renderer(const GPUDevice& rDevice, const Swapchain& rSwapchain);
        ~Renderer();

        vk::Bool32 render();

    private:
        void createSynchronizer();
        void createCommandPool();

        vk::Result acquireImage();
        vk::Result submit();
        vk::Result present();

        const GPUDevice& rDevice;
        const Swapchain& rSwapchain;

        std::vector<vk::Fence> fences;
        std::vector<vk::Semaphore> imageSignal;
        std::vector<vk::Semaphore> renderSignal;
        vk::CommandPool commandPool;
        std::vector<vk::CommandBuffer> commandBuffers;


    };
}
