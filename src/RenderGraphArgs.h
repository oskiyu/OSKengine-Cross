#pragma once

#include "NumericTypes.h"

#include "Vector2.hpp"
#include "Vector3.hpp"

#include "Format.h"
#include "GpuImageDimensions.h"
#include "GpuMemoryTypes.h"
#include "GpuQueueTypes.h"

#include "ImageUuid.h"

#include <string>
#include <optional>
#include "GpuBufferRange.h"

namespace OSK::GRAPHICS {

	/// @brief Resolución de un render-target,
	/// relativa a la resolución de salida.
	/// 
	/// @note Para imágenes 1D, dejar los componentes
	/// `Y = 1.0`, `Z = 1.0`.
	/// @note Para imágenes 2D, dejar el componente
	/// `Z = 1.0`.
	/// 
	/// @example { 1.0, 1.0, 1.0 } = misma resolución.
	/// @example { 0.5, 0.5, 0.5 } = mitad de resolución.
	/// @example { 2.0, 2.0, 2.0 } = resolución doble.
	/// 
	/// @pre @p relativeResolution.x >= 0.
	/// @pre @p relativeResolution.y >= 0.
	/// @pre @p relativeResolution.z >= 0.
	struct OSKAPI_CALL RgRelativeSizeImageRegisterArgs {

		/// @brief Resolución relativa.
		/// @pre @p .x >= 0.
		/// @pre @p .y >= 0.
		/// @pre @p .z >= 0.
		Vector3f relativeResolution = Vector3f::One;

		/// @param resolution Resolución 1D.
		/// @return Estructura inicializada para
		/// una imagen 1D con la resolución indicada.
		/// 
		/// @post @p relativeResolution.y = 1.
		/// @post @p relativeResolution.z = 1.
		static RgRelativeSizeImageRegisterArgs From1D(float resolution);

		/// @param resolution Resolución 2D.
		/// @return Estructura inicializada para
		/// una imagen 2D con la resolución indicada.
		/// 
		/// @post @p relativeResolution.z = 1.
		static RgRelativeSizeImageRegisterArgs From2D(const Vector2f& resolution);

	};


	/// @brief Resolución absoluta de un render-target.
	/// 
	/// @note Para imágenes 1D, dejar los componentes
	/// `Y = 1.0`, `Z = 1.0`.
	/// @note Para imágenes 2D, dejar el componente
	/// `Z = 1.0`.
	/// 
	/// @pre @p resolution.x >= 0.
	/// @pre @p resolution.y >= 0.
	/// @pre @p resolution.z >= 0.
	struct OSKAPI_CALL RgAbsoluteSizeImageRegisterArgs {

		/// @pre @p .x >= 0.
		/// @pre @p .y >= 0.
		/// @pre @p .z >= 0.
		Vector3ui resolution = { 1920, 1080, 1 };

		/// @param resolution Resolución 1D.
		/// @return Estructura inicializada para
		/// una imagen 1D con la resolución indicada.
		/// 
		/// @post @p resolution.y = 1.
		/// @post @p resolution.z = 1.
		static RgAbsoluteSizeImageRegisterArgs From1D(const Vector2ui& resolution);

		/// @param resolution Resolución 2D.
		/// @return Estructura inicializada para
		/// una imagen 2D con la resolución indicada.
		/// 
		/// @post @p resolution.z = 1.
		static RgAbsoluteSizeImageRegisterArgs From2D(const Vector2ui& resolution);

	};


	/// @brief Tipo de resolución indicada
	/// al registrar un nuevo render-target.
	enum class RgImageSizeType {
		ABSOLUTE,
		RELATIVE
	};


	/// @brief Información sobre la resolución
	/// de una imagen.
	struct OSKAPI_CALL RgImageSizeInfo {

		/// @brief Tipo de estructura.
		RgImageSizeType type = RgImageSizeType::RELATIVE;
		Vector3ui absoluteResolution{};
		Vector3f relativeResolution = Vector3f::One;
	};

	/// @brief Información sobre una imagen.
	struct OSKAPI_CALL RgImageRegisterInfo {

		/// @brief Formato de la imagen.
		Format format = Format::UNKNOWN;

		/// @brief Dimensionalidad de la imagen.
		GpuImageDimension dimension = GpuImageDimension::d2D;

		/// @brief Número de capas de la imagen.
		/// @pre @p numLayers > 0.
		USize32 numLayers = 1;

		/// @brief Nombre debug.
		std::optional<std::string> name;

	};

	/// @brief Información sobre un buffer.
	struct OSKAPI_CALL RgBufferRegisterInfo {

		/// @brief Tamaño del buffer,
		/// en bytes.
		/// @pre @p size > 0.
		USize64 size = 0;

		/// @brief Alineación de memoria.
		USize64 alignment = 0;

		/// @brief Tipo de memoria.
		GpuSharedMemoryType sharedType = GpuSharedMemoryType::GPU_ONLY;

		GpuQueueType queue = GpuQueueType::MAIN;

		/// @brief Nombre debug.
		std::optional<std::string> name;

	};


	/// @brief Referencia al rango
	/// de un buffer.
	struct OSKAPI_CALL RgBufferRef {
		GdrBufferUuid bufferUuid;
		GpuBufferRange range;
	};

	/// @brief Referencia a un rango de atributos
	/// de vértice de un tipo en concreto.
	struct OSKAPI_CALL VertexAttributesBufferRef {
		RgBufferRef bufferRef;
		std::string name;
	};

}
