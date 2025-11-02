#pragma once
#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan.hpp>
#include <window.hxx>
#include <optional>

namespace vec {
    class GPUDevice{
    public:
        enum class QueueType{
            eGraphics, ePresent, eTransfer, eCompute
        };
        GPUDevice(const vk::Instance& rInstance, const Window& rWindow);
        ~GPUDevice();

        vk::PhysicalDevice getPhysicalDevice() const { return physicalDevice;}
        vk::PhysicalDeviceMemoryProperties getMemoryProperties() const { return memoryProperties;}
        vk::Device getLogicalDevice() const { return logicalDevice;}
        vk::Queue getQueue(QueueType type) const;
        uint32_t getQueueFamilyIndex(QueueType type) const;

        GPUDevice(const GPUDevice&) = delete;
        GPUDevice& operator=(const GPUDevice&) = delete;

    private:

        void enumeratePhysicalDevice(const Window& rWindow);
        void createLogicalDevice();

        const vk::Instance& rInstance;

        vk::PhysicalDevice physicalDevice;
        vk::PhysicalDeviceMemoryProperties memoryProperties;
        std::optional<uint32_t> graphicsIndex, presentIndex, transferIndex, computeIndex;
        vk::Device logicalDevice;
        vk::Queue graphicsQueue, presentQueue, transferQueue, computeQueue;

    };
}
