#pragma once
#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan.hpp>
#include <window.hxx>
#include <optional>

namespace vec {
    class GPUDevice{
    public:
        GPUDevice(const vk::Instance& rInstance, const Window& rWindow);
        ~GPUDevice();

        GPUDevice(const GPUDevice&) = delete;
        GPUDevice& operator=(const GPUDevice&) = delete;

    private:

        void enumeratePhysicalDevice(const Window& rWindow);
        void createLogicalDevice();

        const vk::Instance& rInstance;

        vk::PhysicalDevice physicalDevice;
        std::optional<uint32_t> graphicsIndex, presentIndex, transferIndex, computeIndex;
        vk::Device logicalDevice;
        vk::Queue graphicsQueue, presentQueue, transferQueue, computeQueue;
    };
}
