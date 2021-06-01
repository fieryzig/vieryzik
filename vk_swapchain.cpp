#include "vk.h"

struct SwapChainSupportDetails
{
	vk::SurfaceCapabilitiesKHR capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR> presentModes;
};

SwapChainSupportDetails _querySwapchainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface)
{
	SwapChainSupportDetails details;
	details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
	details.formats = device.getSurfaceFormatsKHR(surface);
	details.presentModes = device.getSurfacePresentModesKHR(surface);
	return details;
}

template<typename T>
T choose_by_cond(std::vector<T> Ts, std::function<bool(T)> cond, T legacy)
{
	for (const T& t : Ts) {
		if (cond(t)) {
			return t;
		}
	}
	return legacy;
}

vk_swapchain * vk_swapchain_create(
	vk_device * device,
	vk_surface * surface
)
{
	auto ret = new vk_swapchain();
	auto supportDetails = _querySwapchainSupport(device->vkPhysicalDevice, surface->surface);

	auto surfaceFormat = choose_by_cond<vk::SurfaceFormatKHR>(supportDetails.formats, [](auto each) {
		if (each.format == vk::Format::eB8G8R8A8Srgb && each.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) return true;
		return false;
	}, supportDetails.formats[0]);
	auto presentMode = choose_by_cond<vk::PresentModeKHR>(supportDetails.presentModes, [](auto each) {
		if (each == vk::PresentModeKHR::eMailbox) return true;
		return false;
	}, vk::PresentModeKHR::eFifo);
	auto extent = supportDetails.capabilities.currentExtent;
	auto imageCount = supportDetails.capabilities.minImageCount + 1;
	vk::SwapchainCreateInfoKHR info;
	info.setSurface(surface->surface)
		.setMinImageCount(imageCount)
		.setImageFormat(surfaceFormat.format)
		.setImageColorSpace(surfaceFormat.colorSpace)
		.setImageExtent(extent)
		.setImageArrayLayers(1)
		.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
		.setImageSharingMode(vk::SharingMode::eExclusive)
		.setPreTransform(supportDetails.capabilities.currentTransform)
		.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
		.setPresentMode(presentMode)
		.setClipped(vk::Bool32(true))
		.setOldSwapchain(VK_NULL_HANDLE);
	ret->swapchain = device->vkDevice.createSwapchainKHR(info);
	ret->images = device->vkDevice.getSwapchainImagesKHR(ret->swapchain);
	ret->imageFormat = surfaceFormat.format;
	ret->extent = extent;
	return ret;
}
