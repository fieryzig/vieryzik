#include "vk.h"

vk_device*
vk_device_create(vk_instance* vk_inst,
                 const char** extension_names,
                 uint32_t extension_names_count)
{
  auto* ret = new vk_device();

  vk::Instance inst = vk_inst->instance;
  auto deviceList = inst.enumeratePhysicalDevices();
  ret->physicalDevice = deviceList.front();

  std::vector<vk::DeviceQueueCreateInfo> dqc_infos;
  std::vector<const char*> enabledDeviceExtensionNames;

  vk::DeviceCreateInfo deviceCreateInfo;
  deviceCreateInfo.setPNext(nullptr)
    .setQueueCreateInfoCount(uint32_t(dqc_infos.size()))
    .setPQueueCreateInfos(dqc_infos.data())
    .setEnabledLayerCount(0)
    .setPpEnabledLayerNames(nullptr)
    .setEnabledExtensionCount(uint32_t(enabledDeviceExtensionNames.size()))
    .setPpEnabledExtensionNames(enabledDeviceExtensionNames.data())
    .setPEnabledFeatures(nullptr);
  ret->device = ret->physicalDevice.createDevice(deviceCreateInfo);
  return ret;
}

void
vk_device_destroy(vk_device* device)
{
  device->device.destroy();
  delete (device);
}

void
vk_device_wait_idle(vk_device* device)
{
  device->device.waitIdle();
}
