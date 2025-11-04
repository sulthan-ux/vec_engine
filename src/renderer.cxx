#include "device.hxx"
#include "engine.hxx"
#include "vulkan/vulkan.hpp"
#include <renderer.hxx>
#include <stdexcept>

namespace vec{
    Renderer::Renderer(const GPUDevice& rDevice, const Swapchain& rSwapchain, Engine* pEngine) :rDevice(rDevice), rSwapchain(rSwapchain) , pEngine(pEngine) {
        createSynchronizer();
        createCommandPool();
    }
    Renderer::~Renderer(){
        rDevice.getLogicalDevice().waitIdle();
        for(auto semaphore :imageSignal){
            rDevice.getLogicalDevice().destroySemaphore(semaphore);
        }
        for(auto semaphore :renderSignal){
            rDevice.getLogicalDevice().destroySemaphore(semaphore);
        }
        for(auto fence : fences){
            rDevice.getLogicalDevice().destroyFence(fence);
        }
    }

    void Renderer::createSynchronizer(){
        for(uint32_t i = 0; i < 4; ++i){
            vk::FenceCreateInfo fenceInfo = {
                vk::FenceCreateFlagBits::eSignaled,
                nullptr
            };

            vk::Fence fence = {};
            vk::Result result = rDevice.getLogicalDevice().createFence(&fenceInfo, nullptr, &fence);
            if(result != vk::Result::eSuccess){
                throw std::runtime_error("Failed to create fence!!.");
            }
            fences.push_back(fence);

            vk::SemaphoreCreateInfo semaphoreInfo = {
                {},
                nullptr
            };
            vk::Semaphore imageSemaphore = {};
            result = rDevice.getLogicalDevice().createSemaphore(&semaphoreInfo, nullptr, &imageSemaphore);
            if(result != vk::Result::eSuccess){
                throw std::runtime_error("Failed to create semaphore!!.");
            }
            imageSignal.push_back(imageSemaphore);

            vk::Semaphore renderSemaphore = {};
            result = rDevice.getLogicalDevice().createSemaphore(&semaphoreInfo, nullptr, &renderSemaphore);
            if(result != vk::Result::eSuccess){
                throw std::runtime_error("Failed to create semaphore!!.");
            }
            renderSignal.push_back(renderSemaphore);
        }
    }
    void Renderer::createCommandPool(){
        vk::CommandPoolCreateInfo commandPoolInfo = {
            {},
            0,
            nullptr
        };

        vk::Result result = rDevice.getLogicalDevice().createCommandPool(&commandPoolInfo, nullptr, &commandPool);
        if(result != vk::Result::eSuccess){
            throw std::runtime_error("Failed to create command pool!!.");
        }

        commandBuffers.resize(4);
        vk::CommandBufferAllocateInfo bufferInfo = {
            commandPool,
            vk::CommandBufferLevel::ePrimary,
            (uint32_t)commandBuffers.size(),
            nullptr
        };

        result = rDevice.getLogicalDevice().allocateCommandBuffers(&bufferInfo, commandBuffers.data());
        if(result != vk::Result::eSuccess){
            throw std::runtime_error("Failed to create command pool!!.");
        }

    }

    vk::Bool32 Renderer::render(){
        vk:: Result result = rDevice.getLogicalDevice().waitForFences(1, &fences.at(currentFrame), VK_TRUE, UINT64_MAX);
        result = rDevice.getLogicalDevice().resetFences(1, &fences.at(currentFrame));

        result = rDevice.getLogicalDevice().acquireNextImageKHR(rSwapchain.getSwapchain(), UINT64_MAX, imageSignal.at(currentFrame), nullptr, &imageIndex);

        commandBuffers.at(currentFrame).reset({});

        vk::CommandBufferBeginInfo commandBeginInfo = {
            {},
            nullptr,
            nullptr
        };
        commandBuffers.at(currentFrame).begin(commandBeginInfo);

        vk::Rect2D area = {
            {0 ,0},
            rSwapchain.getExtent()
        };
        pEngine->renderNewUI();

        std::array<vk::ClearValue, 2> clearValues = {};
        clearValues.at(0).setColor({0.3f, 0.3f, 0.3f, 1.0f});
        clearValues.at(1).setDepthStencil(1.0f);

        vk::RenderPassBeginInfo renderPassBeginInfo = {
            rSwapchain.getRenderPass(),
            rSwapchain.getFramebuffers().at(imageIndex),
            area,
            clearValues,
            nullptr
        };


        commandBuffers.at(currentFrame).beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);
        pEngine->renderData(commandBuffers.at(currentFrame));
        commandBuffers.at(currentFrame).endRenderPass();
        commandBuffers.at(currentFrame).end();


        vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        vk::SubmitInfo submitInfo = {
            1,
            &imageSignal.at(currentFrame),
            &waitStage,
            1,
            &commandBuffers.at(currentFrame),
            1,
            &renderSignal.at(currentFrame),
            nullptr
        };


        result = rDevice.getQueue(GPUDevice::QueueType::eGraphics).submit(1, &submitInfo, fences.at(currentFrame));

        vk::SwapchainKHR swapchain = rSwapchain.getSwapchain();
        vk::PresentInfoKHR presentInfo = {
            1,
            &renderSignal.at(currentFrame),
            1,
            &swapchain,
            &imageIndex,
            nullptr,
            nullptr
        };
        result = rDevice.getQueue(GPUDevice::QueueType::ePresent).presentKHR(&presentInfo);


        currentFrame = ( currentFrame + 1) % commandBuffers.size();


        return VK_TRUE;
    }

}
