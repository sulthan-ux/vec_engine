#pragma once
#include <vulkan/vulkan.h>
#include <device.hxx>
#include <swapchain.hxx>

namespace vec{
    class Pipeline{
    public:
        Pipeline(const GPUDevice& device, const Swapchain& swapchain);
        ~Pipeline();

    private:
        void createPipelineLayouts();
        vk::ShaderModule createShaderModule(const char* pShaderFile);
        void createGraphicsPipelines();


        const GPUDevice& device;
        const Swapchain& swapchain;

        vk::Pipeline pipeline;
        vk::PipelineLayout layout;
    };
}
