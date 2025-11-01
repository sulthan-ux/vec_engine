#pragma once
#include "vulkan/vulkan.hpp"
#include <device.hxx>
#include <window.hxx>
#include <vector>

namespace vec{
    class Swapchain{
    public:
        Swapchain(const Window& rWindow, const GPUDevice& rDevice);
        ~Swapchain();

    private:
        void createSwapchain();
        void createDepthStencil();

        void recreate();

        const Window& rWindow;
        const GPUDevice& rDevice;

        vk::SwapchainKHR swapchain;
        std::vector<vk::Image> swapchainImages;
        std::vector<vk::ImageView> swapchainImageViews;
        vk::Image depthImage;
        vk::DeviceMemory depthMemory;
        vk::ImageView depthImageView;
    };
}
