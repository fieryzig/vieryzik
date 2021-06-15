#include "vk.h"
#include <GLFW/glfw3.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

int
main()
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  auto window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
  uint32_t glfwExtensionCount = 0;
  auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  std::vector<const char*> extensions(glfwExtensions,
                                      glfwExtensions + glfwExtensionCount);
#ifdef VDEBUK
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
  try {
    auto vk_inst = vk_instance_create(extensions);
    auto createWindowSurface = std::bind(glfwCreateWindowSurface,
                                         vk_inst->instance,
                                         window,
                                         nullptr,
                                         std::placeholders::_1);
    auto vk_surface = vk_surface_create(createWindowSurface);
    vk_instance_destroy(vk_inst);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "UnknownError" << std::endl;
  }
  // auto device = vk_device_create(vk_inst, nullptr, 0);
  // auto pipeline = vk_pipeline_create(device);
  // VkExtent2D extent{};
  // vk_pipeline_prepare_default(pipeline, extent);
  // vk_pipeline_create_graphics_pipeline(pipeline);

  // while (!glfwWindowShouldClose(window)) {
  // 	glfwPollEvents();
  // }

  // vk_device_wait_idle(device);
  // vk_device_destroy(device);

  // vk_surface_destroy(vk_surface, vk_inst);
  return 0;
}
