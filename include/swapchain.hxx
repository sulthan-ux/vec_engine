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

        vk::SwapchainKHR getSwapchain() const { return swapchain;}
        vk::RenderPass getRenderPass() const { return renderPass;}
        std::vector<vk::Framebuffer> getFramebuffers() const { return framebuffers;}

        Swapchain(const Swapchain&) = delete;
        Swapchain& operator=(const Swapchain&) = delete;

    private:
        void createSwapchain();
        void createDepthStencil();
        void createRenderPass();
        void createFramebuffer();

        void recreate();

        const Window& rWindow;
        const GPUDevice& rDevice;

        vk::SwapchainKHR swapchain;
        vk::Extent2D swapchainExtent;
        vk::Format swapchainFormat;
        std::vector<vk::Image> swapchainImages;
        std::vector<vk::ImageView> swapchainImageViews;
        vk::Image depthImage;
        vk::Format depthFormat;
        vk::DeviceMemory depthMemory;
        vk::ImageView depthImageView;
        vk::RenderPass renderPass;
        std::vector<vk::Framebuffer> framebuffers;
    };
}
