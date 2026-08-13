#pragma once

#include "EngineException.h"

namespace OSK::GRAPHICS {

	/// @brief Excepción que se lanza cuando se trata de acceder a un material que no está cargado.
	class OSKAPI_CALL MaterialNotFoundException : public EngineException {

	public:

		explicit MaterialNotFoundException(
			std::string_view name,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("MaterialNotFoundException: El material {} no está cargado.", name),
				location) { }

	};

	/// @brief Excepción que se lanza cuando no se pudo crear el descriptor pool nativo.
	class OSKAPI_CALL DescriptorPoolCreationException : public EngineException {

	public:

		explicit DescriptorPoolCreationException(
			size_t code,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("DescriptorPoolCreationException: Error nativo al crear el desc pool. Code: {}.", code),
				location) { }

	};

	/// @brief Excepción que se lanza cuando no se pudo crear el descriptor pool nativo.
	class OSKAPI_CALL DescriptorLayoutCreationException : public EngineException {

	public:

		explicit DescriptorLayoutCreationException(
			size_t code,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("DescriptorLayoutCreationException: Error nativo al crear el desc layout. Code: {}.", code),
				location) { }

	};

	/// @brief Excepción que se lanza cuando no se pudo crear el material slot.
	class OSKAPI_CALL MaterialSlotCreationException : public EngineException {

	public:

		explicit MaterialSlotCreationException(
			size_t code,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("MaterialSlotCreationException: Error nativo al crear el material slot. Code: {}.", code),
				location) { }

	};

	/// @brief Excepción que se lanza cuando se trata de usar un material no compatible.
	class OSKAPI_CALL InvalidMaterialException : public EngineException {

	public:

		explicit InvalidMaterialException(
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("InvalidMaterialException: Se ha intentado usar un material incompatible."),
				location) { }

	};

}
