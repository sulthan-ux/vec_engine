#pragma once
#include "vulkan/vulkan.hpp"
#include <cstdint>

namespace vec{
    class Application{
    public:
      Application(const char* pApplicationName, uint32_t applicationVersion, VkBool32 enableValidator);
      ~Application();

      void running();

    private:

      void createInstance(vk::ApplicationInfo appInfo ,VkBool32 enableValidator);
      void createDebugMessenger();

      vk::Instance instance;
      vk::DebugUtilsMessengerEXT messenger;

      const char* pEngineName = "Vulkan Ember Core";
      uint32_t engineVersion = VK_MAKE_VERSION(1, 0, 0);
      uint32_t apiVersion = VK_API_VERSION_1_1;

    };
}
