#pragma once

#include "IShaderPass.h"

namespace OSK::GRAPHICS {

	class OSKAPI_CALL [[deprecated("Esta funcionalidad será eliminada en el futuro, usar RenderGraph.")]] 
	BillboardGBufferPass : public IShaderPass {

	public:

		OSK_RENDERPASS(BillboardGBufferPass, "billboard_pass");

		BillboardGBufferPass() : IShaderPass("billboard_pass") {}

		void Load() override;

		void RenderLoop(
			ICommandList* commandList,
			const DynamicArray<ECS::GameObjectIndex>& objectsToRender,
			GlobalMeshMapping* meshMapping,
			UIndex32 jitterIndex,
			Vector2ui resolution) override;

	};

}
