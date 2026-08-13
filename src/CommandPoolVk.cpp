#include "CommandPoolVk.h"

#include "Platforms.h"
#ifdef OSK_USE_VULKAN_BACKEND

#include <vulkan/vulkan.h>

#include "CommandListVk.h"
#include "GpuVk.h"
#include "Assert.h"
#include "QueueFamilyIndices.h"

#include "CommandListExceptions.h"
#include "RendererExceptions.h"

#include "OSKengine.h"
#include "IRenderer.h"

using namespace OSK;
using namespace OSK::GRAPHICS;


CommandPoolVk::CommandPoolVk(const GpuVk& device, const QueueFamily& queueType, GpuQueueType type, USize32 resourcesInFlightCount) : ICommandPool(queueType.support, type), m_logicalDevice(device.GetLogicalDevice()) {
	m_resourcesInFlightCount = resourcesInFlightCount;
	
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueType.familyIndex;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	VkResult result = vkCreateCommandPool(
		device.GetLogicalDevice(), 
		&poolInfo, 
		nullptr, 
		&m_commandPool);

	OSK_ASSERT(result == VK_SUCCESS, CommandPoolCreationException(result));
}

CommandPoolVk::~CommandPoolVk() {
	vkDestroyCommandPool(
		m_logicalDevice,
		m_commandPool, 
		nullptr);
}

UniquePtr<ICommandList> CommandPoolVk::CreateCommandList(const IGpu& device) {
	return CreateList(device, Engine::GetRenderer()->GetSwapchainImagesCount());
}

UniquePtr<ICommandList> CommandPoolVk::CreateSingleTimeCommandList(const IGpu& device) {
	UniquePtr<ICommandList> list = CreateList(device, 1);
	list->_SetSingleTimeUse();

	return list;
}

UniquePtr<ICommandList> CommandPoolVk::CreateList(const IGpu& device, USize32 numNativeLists) {
	return MakeUnique<CommandListVk>(
		*device.As<GpuVk>(),
		this,
		numNativeLists);
}

VkCommandPool CommandPoolVk::GetCommandPool() const {
	return m_commandPool;
}

#endif
