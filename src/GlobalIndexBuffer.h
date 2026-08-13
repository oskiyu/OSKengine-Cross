#pragma once

#include "ApiCall.h"
#include "NumericTypes.h"
#include "DynamicArray.hpp"

namespace OSK::GRAPHICS {

	/// @brief Referencia a una lista
	/// de índices almacenada en el
	/// buffer global de índices.
	struct GlobalIndexBufferRange {
		UIndex32 firstIndex = 0;
		USize32 indexCount = 0;
	};

	/// @brief Referencia a un rango
	/// del buffer de índices que se encuentra
	/// libre, y que puede ser reutilizado.
	struct UnusedIndexBufferRange {
		UIndex32 firstIndex = 0;
		USize32 indexCount = 0;
	};


	/// @brief Maneja los rangos del buffer global
	/// de índices.
	/// 
	/// @note No contiene el propio buffer de GPU.
	class OSKAPI_CALL GlobalIndexBufferManager {

	public:

		/// @brief Devuelve la posición a partir de la que se
		/// puede obtener un rango del buffer con espacio suficiente
		/// para almacenar el número de índices indicado.
		/// @param numIndexes Número de índices a almacenar
		/// en una región contínua de memoria.
		/// @return Posición a partir de la que se puede construir
		/// el rango.
		UIndex32 GetNextIndexesStart(USize32 numIndexes) const;

		/// @brief Reserva un rango del buffer para almacenar
		/// el número de índices indicado, y devuelve la información
		/// del rango.
		/// @param numIndices Número de índices a almacenar.
		/// @return Información sobre la posición y tamaño del
		/// rango reservado.
		GlobalIndexBufferRange RegisterGeometry(USize32 numIndices);

	private:

		DynamicArray<GlobalIndexBufferRange> m_entries;
		DynamicArray<UnusedIndexBufferRange> m_unusedEntries;
		UIndex64 m_tail = 0;

	};

}
