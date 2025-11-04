#include "vulkan/vulkan.hpp"
#include <cstdint>
#include <engine.hxx>
#include <stdexcept>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace vec{
    Engine::Engine(const vk::Instance instance ,const GPUDevice& rDevice, const Window& rWindow, const Swapchain& rSwapchain) : instance(instance), rDevice(rDevice), rWindow(rWindow), rSwapchain(rSwapchain) {
        createDescriptorPool();
        initizalize();
        createMainPipeline();
    }
    Engine::~Engine(){
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

    }
    void Engine::createDescriptorPool(){
        vk::DescriptorPoolSize poolSizes[] = {
            {vk::DescriptorType::eSampler, 200},
            {vk::DescriptorType::eCombinedImageSampler, 200},
            {vk::DescriptorType::eSampledImage, 200},
            {vk::DescriptorType::eStorageImage, 200},
            {vk::DescriptorType::eSampler, 200},
            {vk::DescriptorType::eUniformTexelBuffer, 200},
            {vk::DescriptorType::eStorageTexelBuffer, 200},
            {vk::DescriptorType::eUniformBuffer, 200},
            {vk::DescriptorType::eStorageBuffer, 200},
            {vk::DescriptorType::eUniformBufferDynamic, 200},
            {vk::DescriptorType::eStorageBufferDynamic, 200},
            {vk::DescriptorType::eInputAttachment, 200},
        };
        vk::DescriptorPoolCreateInfo poolInfo = {
            {},
            200 * IM_ARRAYSIZE(poolSizes),
            (uint32_t)IM_ARRAYSIZE(poolSizes),
            poolSizes,
            nullptr
        };

        vk::Result result = rDevice.getLogicalDevice().createDescriptorPool(&poolInfo, nullptr, &descriptorPool);
        if(result != vk::Result::eSuccess){
            throw std::runtime_error("Failed to create engine descriptor pool!!.");
        }
    }
    void Engine::initizalize(){
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplGlfw_InitForVulkan(rWindow.getWindow(), true);

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = instance;
        init_info.PhysicalDevice = rDevice.getPhysicalDevice();
        init_info.Device = rDevice.getLogicalDevice();
        init_info.QueueFamily = rDevice.getQueueFamilyIndex(GPUDevice::QueueType::eGraphics);
        init_info.Queue = rDevice.getQueue(GPUDevice::QueueType::eGraphics);
        init_info.PipelineCache = nullptr;
        init_info.DescriptorPool = descriptorPool;
        init_info.MinImageCount = rSwapchain.getFramebuffers().size();
        init_info.ImageCount = rSwapchain.getFramebuffers().size();
        init_info.Allocator = nullptr;
        init_info.ApiVersion = VK_API_VERSION_1_3;
        init_info.CheckVkResultFn = [](VkResult err){ if (err != VK_SUCCESS) fprintf(stderr, "Vulkan error %d\n", err); };

        ImGui_ImplVulkan_Init(&init_info);
    }
    void Engine::createMainPipeline(){
        std::vector<vk::Format> formats = {vk::Format::eR32G32B32A32Sfloat};
        vk::PipelineRenderingCreateInfo renderingInfo = {
            1,
            formats,
            vk::Format::eD32Sfloat,
            vk::Format::eUndefined,
            nullptr
        };
        ImGui_ImplVulkan_PipelineInfo pipelineInfo = {};
        pipelineInfo.PipelineRenderingCreateInfo = renderingInfo;
        pipelineInfo.MSAASamples = (VkSampleCountFlagBits)vk::SampleCountFlagBits::e1;
        pipelineInfo.RenderPass = rSwapchain.getRenderPass();
        pipelineInfo.Subpass = 0;
        ImGui_ImplVulkan_CreateMainPipeline(&pipelineInfo);
    }
    void Engine::renderNewUI(){
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(250, rSwapchain.getExtent().height));
        ImGui::Begin("Panel Tetap", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize);
        ImGui::Text("Panel ini tidak bisa digeser!");
        ImGui::End();

        ImGui::Render();

    }
    void Engine::renderData(vk::CommandBuffer buffer){
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), buffer);
    }
}
