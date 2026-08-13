#pragma once

#include "DefineAs.h"

namespace OSK::GRAPHICS {

	class OSKAPI_CALL IGpuVertexDescription {

	public:

		virtual ~IGpuVertexDescription() = default;
		OSK_DEFINE_AS(IGpuVertexDescription);

	};

}
