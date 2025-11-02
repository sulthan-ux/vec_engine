#include "device.hxx"
#include "vulkan/vulkan.hpp"
#include <cstdint>
#include <stdexcept>
#include <set>
#include <iostream>

namespace vec{
    GPUDevice::GPUDevice(const vk::Instance& rInstance,  const Window& rWindow) :rInstance(rInstance) {
        enumeratePhysicalDevice(rWindow);
        createLogicalDevice();

    }
    GPUDevice::~GPUDevice(){
        logicalDevice.destroy();
    }
    vk::Queue GPUDevice::getQueue(QueueType type) const{
        switch(type){
            case GPUDevice::QueueType::eGraphics:
                return graphicsQueue;
                break;
            case GPUDevice::QueueType::ePresent:
                return presentQueue;
                break;
            case GPUDevice::QueueType::eTransfer:
                return transferQueue;
                break;
            case GPUDevice::QueueType::eCompute:
                return computeQueue;
                break;
            default:
                break;

        }
        return VK_NULL_HANDLE;
    }
    uint32_t GPUDevice::getQueueFamilyIndex(QueueType type) const{
        switch(type){
            case GPUDevice::QueueType::eGraphics:
                return graphicsIndex.value();
                break;
            case GPUDevice::QueueType::ePresent:
                return presentIndex.value();
                break;
            case GPUDevice::QueueType::eTransfer:
                return transferIndex.value();
                break;
            case GPUDevice::QueueType::eCompute:
                return computeIndex.value();
                break;
            default:
                break;
        }

        return UINT32_MAX;
    }

    void GPUDevice::enumeratePhysicalDevice(const Window& rWindow){
        std::vector<vk::PhysicalDevice> physicalDevices = rInstance.enumeratePhysicalDevices();

        if(physicalDevices.empty()){
            throw std::runtime_error("Cannot get GPU physical devices!!.");
        }

        for( vk::PhysicalDevice device : physicalDevices){
            vk::PhysicalDeviceProperties properties = device.getProperties();

            if((properties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu)||(properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)){
                physicalDevice = device;
            }
        }

        if(physicalDevice == NULL) throw std::runtime_error("Cannot get physicalDevice integrated or discreted!!.");

        std::vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();
        for(uint32_t i = 0; i < queueFamilies.size(); ++i){
            if(queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics){
                this->graphicsIndex = i;
            }
            if(queueFamilies[i].queueFlags & vk::QueueFlagBits::eTransfer){
                this->transferIndex = i;
            }
            if(queueFamilies[i].queueFlags & vk::QueueFlagBits::eCompute){
                this->computeIndex = i;
            }



            vk::Bool32 presentSupport = VK_FALSE;
            vk::Result result = physicalDevice.getSurfaceSupportKHR(i, rWindow.getSurface(), &presentSupport);
            if(!(presentSupport && (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics))){
                throw std::runtime_error("Physical device atleast support graphics and present!!.");
            }
            else{
                this->presentIndex = i;
            }
        }

        memoryProperties = physicalDevice.getMemoryProperties();

    }
    void GPUDevice::createLogicalDevice(){
        std::set<uint32_t> uniqueIndex = { graphicsIndex.value(), presentIndex.value(), transferIndex.value(), computeIndex.value()};
        std::vector<vk::DeviceQueueCreateInfo> queueInfos;
        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueIndex) {
            vk::DeviceQueueCreateInfo queueInfo = {{}, queueFamily, 1, &queuePriority, nullptr};
            queueInfos.push_back(queueInfo);
        }
        std::vector<const char*> layers = {};
        std::vector<const char*> extensions = {"VK_KHR_swapchain"};
        vk::PhysicalDeviceFeatures deviceFeatures = {};

        vk::DeviceCreateInfo deviceInfo = {{}, queueInfos, layers, extensions, &deviceFeatures, nullptr};
        vk::Result result = physicalDevice.createDevice(&deviceInfo, nullptr, &logicalDevice);
        if( result != vk::Result::eSuccess){
            throw std::runtime_error("Failed to create logical device!!.");
        }

        logicalDevice.getQueue(graphicsIndex.value(), 0, &graphicsQueue);
        logicalDevice.getQueue(presentIndex.value(), 0, &presentQueue);
        logicalDevice.getQueue(transferIndex.value(), 0, &transferQueue);
        logicalDevice.getQueue(computeIndex.value(), 0, &computeQueue);
    }
}
