#pragma once

#include "ApiCall.h"
#include "NumericTypes.h"
#include "DynamicArray.hpp"

namespace OSK::GRAPHICS {

	/// @brief Referencia a una lista
	/// de vértices almacenada en el
	/// buffer global de vértices.
	struct GlobalVertexBufferRange {
		UIndex32 firstVertex = 0;
		USize32 vertexCount = 0;
	};

	/// @brief Referencia a un rango
	/// del buffer de vértices que se encuentra
	/// libre, y que puede ser vértices.
	struct UnusedVertexBufferRange {
		UIndex32 firstVertex = 0;
		USize32 vertexCount = 0;
	};


	/// @brief Maneja los rangos del buffer global
	/// de vértices.
	/// 
	/// @note No contiene el propio buffer de GPU.
	class OSKAPI_CALL GlobalVertexBufferManager {

	public:

		/// @brief Reserva un rango del buffer para almacenar
		/// el número de vértices indicado, y devuelve la información
		/// del rango.
		/// @param vertexCount Número de vértices a almacenar.
		/// @return Información sobre la posición y tamaño del
		/// rango reservado.
		GlobalVertexBufferRange RegisterGeometry(USize32 vertexCount);

	private:

		DynamicArray<GlobalVertexBufferRange> m_entries;
		DynamicArray<UnusedVertexBufferRange> m_unusedEntries;
		UIndex32 m_tail = 0;

	};

}
