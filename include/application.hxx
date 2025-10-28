#pragma once
#include "vulkan/vulkan.hpp"
#include <cstdint>

namespace vec{
    class Application{
    public:
      Application(const char* pApplicationName, uint32_t applicationVersion);
      ~Application();

    private:
        vk::Instance instance;
        vk::DebugUtilsMessengerEXT messenger;

        static constexpr vk::Offset2D position = {0,0};
        static constexpr vk::Extent2D size = {800, 600};

    };
}
