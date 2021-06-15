#include "vk.h"

vk_surface*
vk_surface_create(std::function<VkResult(VkSurfaceKHR*)> createWindowSurface)
{
  VkSurfaceKHR vkSurface;
  if (createWindowSurface(&vkSurface) != VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface!");
  }
  auto ret = new vk_surface();
  ret->surface = vk::SurfaceKHR(vkSurface);
  return ret;
}

void
vk_surface_destroy(vk_surface* vk_surf, vk_instance* vk_inst)
{
  vk_inst->instance.destroySurfaceKHR(vk_surf->surface);
  delete (vk_surf);
}
