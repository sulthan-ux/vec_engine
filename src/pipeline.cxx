#include "pipeline.hxx"
#include "device.hxx"
#include "swapchain.hxx"
#include "vulkan/vulkan.hpp"
#include <pipeline.hxx>
#include <stdexcept>
#include <vector>

namespace vec{
    Pipeline::Pipeline(const GPUDevice& device, const Swapchain& swapchain) : device(device), swapchain(swapchain) {
        createPipelineLayouts();
        createGraphicsPipelines();
    }
    Pipeline::~Pipeline(){

    }

    void Pipeline::createPipelineLayouts(){
        vk::PipelineLayoutCreateInfo layoutInfo = {
            {},
            nullptr,
            nullptr,
            nullptr
        };

        vk::Result result = device.getLogicalDevice().createPipelineLayout(&layoutInfo, nullptr, &layout);
        if( result != vk::Result::eSuccess){
            throw std::runtime_error("Failed to create pipeline layout!!.");
        }

    }
    vk::ShaderModule Pipeline::createShaderModule(const char* pShaderFile){

        vk::ShaderModuleCreateInfo moduleInfo = {
            {},
            0,
            nullptr,
            nullptr
        };
        vk::ShaderModule shader = {};

        vk::Result result = device.getLogicalDevice().createShaderModule(&moduleInfo, nullptr, &shader);
        if( result != vk::Result::eSuccess){
            throw std::runtime_error("Failed to create shader module!!.");
            return nullptr;
        }

        return shader;
    }
    void Pipeline::createGraphicsPipelines(){
        vk::PipelineShaderStageCreateInfo vertInfo = {
            {},
            vk::ShaderStageFlagBits::eVertex,
            nullptr,
            "main",
            nullptr,
            nullptr
        };

        std::vector<vk::PipelineShaderStageCreateInfo> shaderInfo;
        shaderInfo.push_back(vertInfo);

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {
            {},
            nullptr,
            nullptr,
            nullptr
        };

        vk::PipelineInputAssemblyStateCreateInfo assemblyInfo = {
            {},
            vk::PrimitiveTopology::eTriangleList,
            vk::False,
            nullptr
        };

        vk::PipelineViewportStateCreateInfo viewportInfo = {
            {},
            1,
            nullptr,
            1,
            nullptr,
            nullptr
        };

        vk::PipelineRasterizationStateCreateInfo rasterInfo = {
            {},
            vk::False,
            vk::False,
            vk::PolygonMode::eFill,
            vk::CullModeFlagBits::eBack,
            vk::FrontFace::eClockwise
        };

        vk::PipelineMultisampleStateCreateInfo multisampleInfo {
            {},
            vk::SampleCountFlagBits::e1
        };

        vk::PipelineDepthStencilStateCreateInfo depthStencilInfo{
            {},
            vk::True,
            vk::False,
            vk::CompareOp::eLess
        };
        std::vector<vk::PipelineColorBlendAttachmentState> colorBlends;
        vk::PipelineColorBlendAttachmentState colorBlend = {
            vk::False
        };
        colorBlends.push_back(colorBlend);

        vk::PipelineColorBlendStateCreateInfo blendInfo {
            {},
            vk::False,
            {},
            colorBlends
        };

        std::vector<vk::DynamicState> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

        vk::PipelineDynamicStateCreateInfo dynamicInfo = {
            {},
            dynamicStates,
            nullptr
        };

        std::vector<vk::GraphicsPipelineCreateInfo> graphicInfos = {};
        vk::GraphicsPipelineCreateInfo graphicsInfo = {
            {},
            shaderInfo,
            &vertexInputInfo,
            &assemblyInfo,
            nullptr,
            &viewportInfo,
            &rasterInfo,
            &multisampleInfo,
            &depthStencilInfo,
            &blendInfo,
            &dynamicInfo,
            layout,
            swapchain.getRenderPass(),
            0,
            {},
            {},
            nullptr

        };
        graphicInfos.push_back(graphicsInfo);

        vk::Result result = device.getLogicalDevice().createGraphicsPipelines(nullptr, graphicInfos, nullptr);
    }

}
