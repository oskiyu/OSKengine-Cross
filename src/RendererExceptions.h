#pragma once

#include "EngineException.h"

namespace OSK::GRAPHICS {

	/// @brief Excepción que se lanza cuando se trata de acceder a funcionalidades de
	/// trazado de rayos que no están disponibles en la plataforma o no han sido activadas.
	class OSKAPI_CALL RayTracingNotSupportedException : public EngineException {

	public:

		explicit RayTracingNotSupportedException(
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("RayTracingNotSupportedException: Se ha intentado acceder a funcionalidad no disponible."),
				location) { }

	};

	class OSKAPI_CALL RendererCreationException : public EngineException {

	public:

		explicit RendererCreationException(
			std::string_view msg,
			size_t code,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("RendererCreationException: Error: {}. Code: {}.", msg, code),
				location) { }

	};

	class OSKAPI_CALL GpuNotCompatibleException : public EngineException {

	public:

		explicit GpuNotCompatibleException(
			std::string_view msg,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("La GPU no es compatible {}.", msg),
				location) { }

	};

	class OSKAPI_CALL GpuNotFoundException : public EngineException {

	public:

		explicit GpuNotFoundException(
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("No hay ninguna GPU compatible."),
				location) { }

	};

	class OSKAPI_CALL CommandListSubmitException : public EngineException {

	public:

		explicit CommandListSubmitException(
			std::string_view listName,
			size_t code,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("No hay ninguna GPU compatible."),
				location) { }

	};

	class OSKAPI_CALL CommandQueueSubmitException : public EngineException {

	public:

		explicit CommandQueueSubmitException(
			size_t code,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("No hay ninguna GPU compatible."),
				location) { }

	};

	class OSKAPI_CALL CommandPoolCreationException : public EngineException {

	public:

		explicit CommandPoolCreationException(
			size_t code,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("CommandPoolCreationException: No se pudo crear el command pool. Code: {}.", code),
				location) { }

	};

	class OSKAPI_CALL LogicalDeviceCreationException : public EngineException {

	public:

		explicit LogicalDeviceCreationException(
			size_t code,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("LogicalDeviceCreationException: No se pudo realizar la conexión lógica con la GPU. Code: {}.", code),
				location) { }

	};

	class OSKAPI_CALL ImageCreationException : public EngineException {

	public:

		explicit ImageCreationException(
			size_t code,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("ImageCreationException: No se pudo crear la imagen. Code: {}.", code),
				location) { }

	};

	class OSKAPI_CALL ImageViewCreationException : public EngineException {

	public:

		explicit ImageViewCreationException(
			size_t code,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("ImageViewCreationException: No se pudo crear el image view. Code: {}.", code),
				location) { }

	};

	class OSKAPI_CALL SwapchainCreationException : public EngineException {

	public:

		explicit SwapchainCreationException(
			std::string_view msg,
			size_t code,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("SwapchainCreationException: {}. Code: {}.", msg, code),
				location) { }

	};

	class OSKAPI_CALL GpuGenericException : public EngineException {

	public:

		explicit GpuGenericException(
			std::string_view msg,
			int code,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("GpuGenericException: {}. Code: {}.", msg, code),
				location) { }

	};

}
