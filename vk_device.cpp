#include "vk.h"

struct vk_device
{
	vk::Device			vkDevice = nullptr;
	vk::PhysicalDevice	vkPhysicalDevice = nullptr;
};

vk_device * vk_device_create(vk_instance * vk_inst, const char ** extension_names, uint32_t extension_names_count)
{
	auto* ret = new vk_device();

	vk::Instance inst = vk_instance_get_instance(vk_inst);
	auto deviceList = inst.enumeratePhysicalDevices();
	ret->vkPhysicalDevice = deviceList.front();

	std::vector<vk::DeviceQueueCreateInfo> dqc_infos;
	std::vector<const char*> enabledDeviceExtensionNames;

	vk::DeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo
		.setPNext(nullptr)
		.setQueueCreateInfoCount(uint32_t(dqc_infos.size()))
		.setPQueueCreateInfos(dqc_infos.data())
		.setEnabledLayerCount(0)
		.setPpEnabledLayerNames(nullptr)
		.setEnabledExtensionCount(uint32_t(enabledDeviceExtensionNames.size()))
		.setPpEnabledExtensionNames(enabledDeviceExtensionNames.data())
		.setPEnabledFeatures(nullptr);
	ret->vkDevice = ret->vkPhysicalDevice.createDevice(deviceCreateInfo);
	return ret;
}

void vk_device_destroy(vk_device *device)
{
	device->vkDevice.destroy();
	delete(device);
}

VkDevice_T * vk_device_get_device(vk_device *device)
{
	return device->vkDevice;
}

void vk_device_wait_idle(vk_device * device)
{
	device->vkDevice.waitIdle();
}

