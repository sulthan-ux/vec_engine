#include "vulkan/vulkan.hpp"
#include <swapchain.hxx>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace vec{
    Swapchain::Swapchain(const Window& rWindow, const GPUDevice& rDevice) :rWindow(rWindow), rDevice(rDevice){
        createSwapchain();
        createDepthStencil();
        createRenderPass();
        createFramebuffer();
    }
    Swapchain::~Swapchain(){
        for( auto framebuffer : framebuffers){
            rDevice.getLogicalDevice().destroyFramebuffer(framebuffer);
        }
        rDevice.getLogicalDevice().destroyRenderPass(renderPass);
        rDevice.getLogicalDevice().destroyImageView(depthImageView);
        rDevice.getLogicalDevice().destroyImage(depthImage);
        rDevice.getLogicalDevice().freeMemory(depthMemory);
        for( auto imageView : swapchainImageViews){
            rDevice.getLogicalDevice().destroyImageView(imageView);
        }
        rDevice.getLogicalDevice().destroySwapchainKHR(swapchain);
    }
    void Swapchain::createSwapchain(){
        vk::SurfaceCapabilitiesKHR capabilities = rDevice.getPhysicalDevice().getSurfaceCapabilitiesKHR(rWindow.getSurface());
        std::vector<vk::SurfaceFormatKHR> formats = rDevice.getPhysicalDevice().getSurfaceFormatsKHR(rWindow.getSurface());
        std::vector<vk::PresentModeKHR> presentModes = rDevice.getPhysicalDevice().getSurfacePresentModesKHR(rWindow.getSurface());

        uint32_t imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) imageCount = capabilities.maxImageCount;

        swapchainExtent = capabilities.currentExtent;
        swapchainFormat = formats.front().format;

        vk::SwapchainCreateInfoKHR swapchainInfo = {};
        swapchainInfo.setSurface(rWindow.getSurface());
        swapchainInfo.setMinImageCount(imageCount);
        swapchainInfo.setImageFormat(swapchainFormat);
        swapchainInfo.setImageColorSpace(formats.front().colorSpace);
        swapchainInfo.setImageExtent(swapchainExtent);
        swapchainInfo.setImageArrayLayers(1);
        swapchainInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
        swapchainInfo.setPresentMode(vk::PresentModeKHR::eFifo);

        std::vector<uint32_t> familyIndices = {rDevice.getQueueFamilyIndex(GPUDevice::QueueType::eGraphics), rDevice.getQueueFamilyIndex(GPUDevice::QueueType::ePresent)};



        if(rDevice.getQueueFamilyIndex(GPUDevice::QueueType::eGraphics) != rDevice.getQueueFamilyIndex(GPUDevice::QueueType::ePresent)){
            swapchainInfo.setImageSharingMode(vk::SharingMode::eConcurrent);
            swapchainInfo.setQueueFamilyIndexCount(2);
            swapchainInfo.setQueueFamilyIndices(familyIndices);
        }
        else{
            swapchainInfo.setImageSharingMode(vk::SharingMode::eExclusive);
        }
        swapchainInfo.setPreTransform(capabilities.currentTransform);
        swapchainInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);
        swapchainInfo.setClipped(VK_TRUE);
        swapchainInfo.setOldSwapchain(nullptr);

        vk::Result result = rDevice.getLogicalDevice().createSwapchainKHR(&swapchainInfo, nullptr, &swapchain);
        if(result != vk::Result::eSuccess){
            throw std::runtime_error("Failed to create swapchain!!.");
        }

        swapchainImages = rDevice.getLogicalDevice().getSwapchainImagesKHR(swapchain);

        for( auto image : swapchainImages){
            vk::ImageViewCreateInfo imageViewInfo = {};
            imageViewInfo.setViewType(vk::ImageViewType::e2D);
            imageViewInfo.setImage(image);
            imageViewInfo.setComponents({vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity,});
            vk::ImageSubresourceRange subResourceRange = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1};
            imageViewInfo.setSubresourceRange(subResourceRange);

            vk::ImageView imageView = {};
            vk::Result result = rDevice.getLogicalDevice().createImageView(&imageViewInfo, nullptr, &imageView);
            if(result != vk::Result::eSuccess){
                throw std::runtime_error("Failed to create swapchain image view!!.");
            }

            swapchainImageViews.push_back(imageView);
        }


    }
    void Swapchain::createDepthStencil(){
        depthFormat = vk::Format::eD32Sfloat;
        vk::ImageCreateInfo imageInfo = {
            {},vk::ImageType::e2D,
            depthFormat,
            {swapchainExtent.width, swapchainExtent.height, 1},
            1,
            1,
            vk::SampleCountFlagBits::e1,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eDepthStencilAttachment,
            vk::SharingMode::eExclusive,
            {},
            nullptr,
            vk::ImageLayout::eUndefined,
            nullptr
        };

        vk::Result result = rDevice.getLogicalDevice().createImage(&imageInfo, nullptr, &depthImage);
        if(result != vk::Result::eSuccess){
            throw std::runtime_error("Failed to create depth image");
        }

        vk::MemoryRequirements requirements = rDevice.getLogicalDevice().getImageMemoryRequirements(depthImage);
        uint32_t memoryIndex = {};
        for (uint32_t i = 0; i < rDevice.getMemoryProperties().memoryTypeCount; i++) {
            if ((requirements.memoryTypeBits & (1 << i)) &&
                (rDevice.getMemoryProperties().memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal) == vk::MemoryPropertyFlagBits::eDeviceLocal) {
                memoryIndex = i;
                }
        }

        vk::MemoryAllocateInfo memoryInfo = {requirements.size, memoryIndex, nullptr};

        result = rDevice.getLogicalDevice().allocateMemory(&memoryInfo, nullptr, &depthMemory);
        if(result != vk::Result::eSuccess){
            throw std::runtime_error("Failed to allocate depth memory");
        }
        rDevice.getLogicalDevice().bindImageMemory(depthImage, depthMemory, 0);

        vk::ImageViewCreateInfo imageViewInfo = {};
        imageViewInfo.setViewType(vk::ImageViewType::e2D);
        imageViewInfo.setImage(depthImage);
        vk::ImageSubresourceRange subResourceRange = {vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1};
        imageViewInfo.setSubresourceRange(subResourceRange);

        result = rDevice.getLogicalDevice().createImageView(&imageViewInfo, nullptr, &depthImageView);
        if(result != vk::Result::eSuccess){
            throw std::runtime_error("Failed to create depth image view!!.");
        }

    }
    void Swapchain::createRenderPass(){
        vk::AttachmentDescription colorAttachment{
            {},
            swapchainFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::ePresentSrcKHR
        };

        vk::AttachmentDescription depthAttachment{
            {},
            depthFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eDontCare,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eDepthStencilAttachmentOptimal
        };

        vk::AttachmentReference colorAttachmentRef{
            0, // index ke-0 = colorAttachment
            vk::ImageLayout::eColorAttachmentOptimal
        };

        vk::AttachmentReference depthAttachmentRef{
            1, // index ke-1 = depthAttachment
            vk::ImageLayout::eDepthStencilAttachmentOptimal
        };

        vk::SubpassDescription subpass{
            {},
            vk::PipelineBindPoint::eGraphics,     // subpass untuk graphics
            0, nullptr,                           // input attachment
            1, &colorAttachmentRef,               // color attachment
            nullptr,                              // resolve attachment
            &depthAttachmentRef,                  // depth attachment
            0, nullptr                            // preserve attachment
        };

        vk::SubpassDependency dependency{
            VK_SUBPASS_EXTERNAL,  // dari luar renderpass
            0,                    // ke subpass 0
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::AccessFlagBits::eNone,
            vk::AccessFlagBits::eColorAttachmentWrite
        };

        std::array<vk::AttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

        vk::RenderPassCreateInfo renderPassInfo{
            {},
            static_cast<uint32_t>(attachments.size()),
            attachments.data(),
            1, &subpass,
            1, &dependency
        };

        vk::Result result = rDevice.getLogicalDevice().createRenderPass(&renderPassInfo, nullptr, &renderPass);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("failed to create render pass!");
        }

    }
    void Swapchain::createFramebuffer(){
        for( vk::ImageView imageView : swapchainImageViews){
            std::array<vk::ImageView, 2> imageViews = { imageView, depthImageView};

            vk::FramebufferCreateInfo framebufferInfo = {
                {},
                renderPass,
                imageViews,
                swapchainExtent.width,
                swapchainExtent.height,
                1,
                nullptr
            };

            vk::Framebuffer framebuffer = {};
            vk::Result result = rDevice.getLogicalDevice().createFramebuffer(&framebufferInfo, nullptr, &framebuffer);
            if (result != vk::Result::eSuccess) {
                throw std::runtime_error("failed to create framebuffer!!");
            }

            framebuffers.push_back(framebuffer);
        }
    }

    void recreate(){

    }
}
