#pragma once
#include "swapchain.hxx"
#include <vulkan/vulkan.hpp>
#include <device.hxx>

namespace vec{
    class Engine{
    public:
        Engine(const vk::Instance instance ,const GPUDevice& rDevice, const Window& rWindow, const Swapchain& rSwapchain);
        ~Engine();

        void renderNewUI();
        void renderData(vk::CommandBuffer buffer);

    private:

        void createDescriptorPool();
        void initizalize();
        void createMainPipeline();

        const vk::Instance instance;
        const GPUDevice& rDevice;
        const Window& rWindow;
        const Swapchain& rSwapchain;

        vk::DescriptorPool descriptorPool;
    };
}
