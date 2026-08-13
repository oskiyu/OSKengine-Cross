#pragma once

#include "Platforms.h"
#ifdef OSK_USE_VULKAN_BACKEND

#include <vulkan/vulkan.h>

#include "ApiCall.h"

namespace OSK::GRAPHICS {

	enum class ShaderBindingType;
	enum class ShaderStage;

	VkDescriptorType OSKAPI_CALL GetDescriptorTypeVk(ShaderBindingType type);

	VkShaderStageFlags OSKAPI_CALL GetShaderStageVk(ShaderStage stage);

}

#endif
