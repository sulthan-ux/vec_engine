#include "device.hxx"
#include "vulkan/vulkan.hpp"
#include <stdexcept>
#include <iostream>
namespace vec{
    GPUDevice::GPUDevice(const vk::Instance& rInstance,  const Window& rWindow) :rInstance(rInstance) {
        enumeratePhysicalDevice(rWindow);
        createLogicalDevice();

    }
    GPUDevice::~GPUDevice(){

        //logicalDevice.destroy();
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
        }


    }
    void GPUDevice::createLogicalDevice(){

    }
}
