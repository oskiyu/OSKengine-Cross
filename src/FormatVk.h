#pragma once

#include "Platforms.h"
#ifdef OSK_USE_VULKAN_BACKEND

#include <vulkan/vulkan.h>

namespace OSK::GRAPHICS {
	
	enum class Format;

	VkFormat OSKAPI_CALL GetFormatVk(Format format);
	Format OSKAPI_CALL FormatVkToEngine(VkFormat format);
}

#endif
