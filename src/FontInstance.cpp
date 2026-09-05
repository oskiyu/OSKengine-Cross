#include "FontInstance.h"

#include "IGpuImage.h"
#include "FontCharacter.h"

using namespace OSK;
using namespace OSK::ASSETS;
using namespace OSK::GRAPHICS;

constexpr auto SPACES_PER_TAB = 4;

Vector2f FontInstance::GetTextSize(std::string_view string) const {
	//Char que se tomará como referencia en cuanto al tamaño de las letras.
	const FontCharacter& reference = characters.at('A');

	if (string == "") {
		return Vector2f::Zero;
	}

	float temporalSizeX = 0.0f; // Tamaño de la linea procesada en un momento dado.
	float sizeX = 0.0f;
	USize32 lineCount = 1;

	for (const auto c : string) {
		const auto& character = characters.at(c);

		switch (c) {

		case '\n':
			sizeX = glm::max(sizeX, temporalSizeX);
			temporalSizeX = 0;
			lineCount++;
			break;

		case '\t':
			temporalSizeX += (character.bearing.x * fontSize + character.size.x * fontSize) * SPACES_PER_TAB;
			break;

		case ' ':
			temporalSizeX += (reference.bearing.x * fontSize + reference.size.x * fontSize);
			break;

		default:
			temporalSizeX += character.bearing.x * fontSize + character.size.x * fontSize;
			break;

		}
	}

	sizeX = glm::max(sizeX, temporalSizeX);

	return Vector2f(sizeX, reference.size.y + reference.bearing.y);
}
