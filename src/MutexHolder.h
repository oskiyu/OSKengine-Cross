#pragma once

#include <mutex>
#include "ApiCall.h"

namespace OSK {

	/// @brief Estructura que contiene un mutex,
	/// de tal manera que permite usarlo como atributo de una
	/// clase sin invalidar las operaciones de copia
	/// y movimiento.
	struct OSKAPI_CALL MutexHolder {

		std::mutex mutex{};

		MutexHolder() = default;
		~MutexHolder() = default;

		MutexHolder(const MutexHolder&) {};
		MutexHolder& operator=(const MutexHolder&) { return *this; };

	};

}
