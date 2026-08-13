#pragma once

#include "ApiCall.h"

#include <string>

namespace OSK::GRAPHICS {

	/// @brief Interfaz común para los objetos almacenados en la CPU.
	class OSKAPI_CALL IGpuObject {

	public:

		virtual ~IGpuObject() = default;

		/// @brief Establece un nombre que lo identifique.
		virtual void SetDebugName(const std::string& name) = 0;

	};

}
