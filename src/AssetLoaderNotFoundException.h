#pragma once

#include "EngineException.h"

namespace OSK::ASSETS {

	/// @brief Excepción que se lanza cuando se trata de cargar un asset cuando no hay registrado
	/// ningún loader capaz de procesarlo.
	/// Asegúrese de cargar previamente el loader.
	class OSKAPI_CALL AssetLoaderNotFoundException : public EngineException {

	public:

		explicit AssetLoaderNotFoundException(
			std::string_view assetType,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("AssetLoaderNotFoundException: El loader para el recurso del tipo {} no ha sido registrado.", assetType),
				location) { }

	};

	/// @brief Excepción que se lanza cuando se trata de registrar un asset previamente registrado.
	class OSKAPI_CALL AssetLoaderAlreadyRegisteredException : public EngineException {

	public:

		explicit AssetLoaderAlreadyRegisteredException(
			std::string_view assetType,
			const std::source_location& location = std::source_location::current())
			:
			EngineException(
				std::format("AssetLoaderAlreadyRegisteredException: El loader para el recurso del tipo {} ya ha sido registrado.", assetType),
				location) { }

	};

}
