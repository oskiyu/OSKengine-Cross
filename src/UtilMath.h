#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <type_traits>

#include "Serializer.h"

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.h"

namespace OSK::MATH {

	/// @brief Concepto que permite distinguir cualquier tipo numérico.
	/// @note Tipos de coma flotante: `float`, `double`, `long double`.
	/// @note Tipos enteros: `bool`, `char`, `unsigned char`, `float`, `short`,
	/// `unsigned short`, `int`, `unsigned int`, `long`, `unsigned long`, `long long`,
	/// `unsigned long long`.
	template <typename T> concept IsNumeric = std::is_floating_point_v<T> && std::is_integral_v<T>;

	/// @brief Concepto que permite distinguir cualquier tipo numérico entero.
	/// @note Tipos enteros: `bool`, `char`, `unsigned char`, `float`, `short`,
	/// `unsigned short`, `int`, `unsigned int`, `long`, `unsigned long`, `long long`,
	/// `unsigned long long`.
	template <typename T> concept IsInteger = std::is_integral_v<T>;

	/// @brief Devuelve el primer múltiplo de `multiplo` que sea igual o mayor que ´número´.
	/// @tparam T Tipo de dato.
	/// @param numero Número por el que se empieza la búsqueda.
	/// @param multiplo Múltiplo buscado.
	/// @return Múltiplo mayor o igual que el número.
	/// 
	/// @pre El tipo de dato debe cumplir IsInteger.
	template <typename T> inline T PrimerMultiploSuperior(T numero, T multiplo) {
		T resto = numero % multiplo;
		if (numero % multiplo == 0)
			return numero;

		return (numero / multiplo + 1) * multiplo;
	}

	/// @brief Devuelve el primer múltiplo de `multiplo` que sea igual o menor que ´número´.
	/// @tparam T Tipo de dato.
	/// @param numero Número por el que se empieza la búsqueda.
	/// @param multiplo Múltiplo buscado.
	/// @return Múltiplo menor o igual que el número.
	/// 
	/// @pre El tipo de dato debe cumplir IsInteger.
	template <typename T> T PrimerMultiploInferior(T numero, T multiplo) {
		T resto = numero % multiplo;
		if (numero % multiplo == 0)
			return numero;

		return (numero / multiplo - 1) * multiplo;
	}

	/// @brief Interpolación lineal rápida.
	/// Para cuando factor está entre 0.0 y 1.0
	/// @tparam T 
	/// @param a Valor devuelto cuando @p factor sea 0.0.
	/// @param b Valor devuelto cuando @p factor sea 1.0.
	/// @param factor Factor que permite elegir la interpolación entre @p a y @p b.
	/// @return Valor linealmente interpolado.
	/// 
	/// @warning Si factor está fuera del rango 0-1, el valor devuelto no estará
	/// entre a y b.
	/// 
	/// @pre El tipo @p T debe tener definidos los operadores -, + y *.
	template <typename T> inline T LinearInterpolation_Fast(const T& a, const T& b, float factor) {
		return (a * (1.0f - factor)) + (b * factor);
	}

	/// @brief Interpolación lineal.
	/// @tparam T T Tipo de dato.
	/// @param a Valor devuelto cuando factor sea <= 0.0.
	/// @param b Valor devuelto cuando factor sea >= 1.0.
	/// @param factor  Factor que permite elegir la interpolación entre a y b.
	/// @return Valor linealmente interpolado.
	/// 
	/// @pre El tipo @p T debe tener definidos los operadores >=, <=, -, + y *.
	template <typename T> inline T LinearInterpolation(const T& a, const T& b, float factor) {
		if (factor >= 1.0f)
			return b;

		if (factor <= 0.0f)
			return a;

		return (a * (1.0f - factor)) + (b * factor);
	}

	/// @brief Devuelve el valor, ajustándolo de manera que nunca sea mayor que
	/// el máximo ni menor que el mínimo.
	/// @tparam T Tipo de dato.
	/// @param value Valor al que se le aplicará los límites.
	/// @param min Límite inferior.
	/// @param max Límite superior.
	/// 
	/// @pre El tipo @p T debe tener definidos los operadores > y <.
	template <typename T> inline T Clamp(const T& value, const T& min, const T& max) {
		if (value > max)
			return max;

		if (value < min)
			return min;

		return value;
	}

	/// @brief Para dos matrices modelo, devuelve una matriz con la diferencia de posición, rotación y escala entra ambas.
	/// @param first Matriz modelo A.
	/// @param second Matriz modelo B.
	/// @return Offset de @p B respecto a @p A ( @p A - @p B ).
	static inline glm::mat4 GetMatrixOffset(const glm::mat4& first, const glm::mat4& second) {
		return glm::inverse(first) - second;
	}
	
	/// @tparam T Tipo de dato.
	/// @param value Valor a comprobar.
	/// @return 1 si es mayor o igual que 0, -1 si es menor que 0.
	/// 
	/// @pre El tipo @p T debe tener definidos los operadores >=, <=, -, + y *.
	template <typename T> T Sign(T value) {
		return value >= static_cast<T>(0) ? static_cast<T>(1) : static_cast<T>(-1);
	}

}


namespace OSK::PERSISTENCE {

	template <>
	nlohmann::json inline SerializeData<glm::mat3>(const glm::mat3& data) {
		nlohmann::json output{};

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				output[std::to_string(i) + std::to_string(j)] = data[i][j];
			}
		}

		return output;
	}

	template <>
	glm::mat3 inline DeserializeData<glm::mat3>(const nlohmann::json& json) {
		glm::mat3 output{};

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				output[i][j] = json[std::to_string(i) + std::to_string(j)];
			}
		}

		return output;
	}

	template<>
	PERSISTENCE::BinaryBlock inline BinarySerializeData<glm::mat3>(const glm::mat3& data) {
		auto output = BinaryBlock::Empty();

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				output.Write(data[i][j]);
			}
		}

		return output;
	}

	template<>
	glm::mat3 inline BinaryDeserializeData<glm::mat3>(BinaryBlockReader* data) {
		glm::mat3 output{};

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				output[i][j] = data->Read<float>();
			}
		}

		return output;
	}


	template <>
	nlohmann::json inline SerializeData<glm::mat4>(const glm::mat4& data) {
		nlohmann::json output{};

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				output[std::to_string(i) + std::to_string(j)] = data[i][j];
			}
		}

		return output;
	}

	template <>
	glm::mat4 inline DeserializeData<glm::mat4>(const nlohmann::json& json) {
		glm::mat4 output{};

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				output[i][j] = json[std::to_string(i) + std::to_string(j)];
			}
		}

		return output;
	}

	template<>
	PERSISTENCE::BinaryBlock inline BinarySerializeData<glm::mat4>(const glm::mat4& data) {
		auto output = BinaryBlock::Empty();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				output.Write(data[i][j]);
			}
		}

		return output;
	}

	template<>
	glm::mat4 inline BinaryDeserializeData<glm::mat4>(BinaryBlockReader* data) {
		glm::mat4 output{};

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				output[i][j] = data->Read<float>();
			}
		}

		return output;
	}

#pragma region Vectors

	template <typename TVec>
	inline nlohmann::json SerializeVector2(const TVec& vec) {
		nlohmann::json output{};

		output["x"] = vec.x;
		output["y"] = vec.y;

		return output;
	}

	template <typename TVec>
	inline nlohmann::json SerializeVector3(const TVec& vec) {
		nlohmann::json output{};

		output["x"] = vec.x;
		output["y"] = vec.y;
		output["z"] = vec.z;

		return output;
	}

	template <typename TVec>
	inline nlohmann::json SerializeVector4(const TVec& vec) {
		nlohmann::json output{};

		output["x"] = vec.x;
		output["y"] = vec.y;
		output["z"] = vec.z;
		output["W"] = vec.W;

		return output;
	}


	template <typename TVec>
	inline TVec DeserializeVector2(const nlohmann::json& vec) {
		TVec output{};

		output.x = vec["x"];
		output.y = vec["y"];

		return output;
	}

	template <typename TVec>
	inline TVec DeserializeVector3(const nlohmann::json& vec) {
		TVec output{};

		output.x = vec["x"];
		output.y = vec["y"];
		output.z = vec["z"];

		return output;
	}

	template <typename TVec>
	inline TVec DeserializeVector4(const nlohmann::json& vec) {
		TVec output{};

		output.x = vec["x"];
		output.y = vec["y"];
		output.z = vec["z"];
		output.W = vec["W"];

		return output;
	}


	template <typename TVec>
	inline PERSISTENCE::BinaryBlock SerializeBinaryVector2(const TVec& vec) {
		PERSISTENCE::BinaryBlock output{};

		output.Write(vec.x);
		output.Write(vec.y);

		return output;
	}

	template <typename TVec>
	inline PERSISTENCE::BinaryBlock SerializeBinaryVector3(const TVec& vec) {
		PERSISTENCE::BinaryBlock output{};

		output.Write(vec.x);
		output.Write(vec.y);
		output.Write(vec.z);

		return output;
	}

	template <typename TVec>
	inline PERSISTENCE::BinaryBlock SerializeBinaryVector4(const TVec& vec) {
		PERSISTENCE::BinaryBlock output{};

		output.Write(vec.x);
		output.Write(vec.y);
		output.Write(vec.z);
		output.Write(vec.w);

		return output;
	}

	template <typename TVec, typename TNumberType>
	inline TVec DeserializeBinaryVector2(PERSISTENCE::BinaryBlockReader* vec) {
		TVec output{};

		output.x = vec->Read<TNumberType>();
		output.y = vec->Read<TNumberType>();

		return output;
	}

	template <typename TVec, typename TNumberType>
	inline TVec DeserializeBinaryVector3(PERSISTENCE::BinaryBlockReader* vec) {
		TVec output{};

		output.x = vec->Read<TNumberType>();
		output.y = vec->Read<TNumberType>();
		output.z = vec->Read<TNumberType>();

		return output;
	}

	template <typename TVec, typename TNumberType>
	inline TVec DeserializeBinaryVector4(PERSISTENCE::BinaryBlockReader* vec) {
		TVec output{};

		output.x = vec->Read<TNumberType>();
		output.y = vec->Read<TNumberType>();
		output.z = vec->Read<TNumberType>();
		output.W = vec->Read<TNumberType>();

		return output;
	}

#pragma endregion


	template <>
	nlohmann::json inline SerializeData<Quaternion>(const Quaternion& data) {
		return SerializeData<glm::mat4>(data.ToMat4());

	}

	template <>
	Quaternion inline DeserializeData<Quaternion>(const nlohmann::json& json) {
		return Quaternion{};
		const auto matrix = DeserializeData<glm::mat4>(json);
		return Quaternion::FromGlm(glm::normalize(glm::toQuat(matrix)));
	}


	template <>
	PERSISTENCE::BinaryBlock inline BinarySerializeData<Quaternion>(const Quaternion& data) {
		return BinarySerializeData<glm::mat4>(data.ToMat4());

	}

	template <>
	Quaternion inline BinaryDeserializeData<Quaternion>(PERSISTENCE::BinaryBlockReader* reader) {
		// return Quaternion{};
		const auto matrix = BinaryDeserializeData<glm::mat4>(reader);
		return Quaternion::FromGlm(glm::normalize(glm::toQuat(matrix)));
	}

}
