#include "UiTextView.h"

#include "SpriteRenderer.h"
#include "TransformComponent2D.h"

#include "Font.h"
#include "FontInstance.h"

#include "SdfStringInfo.h"
#include "SdfBindlessRenderer2D.h"
#include "GameObject.h"

#include "OSKengine.h"
#include "Logger.h"

using namespace OSK;
using namespace OSK::UI;
using namespace OSK::ECS;
using namespace OSK::ASSETS;
using namespace OSK::GRAPHICS;

void TextView::AdjustSizeToText() {
	if (!font.GetAsset()) {
		return;
	}

	const auto& fontInstance = font->GetInstance(m_resizedFontSize);
	const auto& referenceChar = fontInstance.characters.at('A');

	float currentSizeX = 0.0f;
	float totalSizeX = 0.0f;
	float totalSizeY = referenceChar.size.y;

	for (const char c : text) {
		if (c == '\n') {
			totalSizeX = glm::max(totalSizeX, currentSizeX);
			currentSizeX = 0.0f;
			totalSizeY += referenceChar.size.y + referenceChar.bearing.y;

			continue;
		}

		if (c == '\t') {
			currentSizeX += (referenceChar.advance >> 6) * 4;
			continue;
		}

		if (c == ' ') {
			currentSizeX += (referenceChar.advance >> 6);
			continue;
		}

		currentSizeX += fontInstance.characters.at(c).advance >> 6;
	}

	totalSizeX = glm::max(totalSizeX, currentSizeX);
	
	m_textSize = Vector2f(
		totalSizeX,
		totalSizeY
	);

	const auto previousSize = m_size;
	m_size = *m_textSize + GetPadding2D();

	// Esto lo último, para que las draw calls
	// tengan el tamaño actualizado.
	IElement::OnSizeChanged(previousSize);
}

void TextView::SetPadding(const Vector4f& padding) {
	IElement::SetPadding(padding);

	if (m_textSize) {
		SetSize(*m_textSize + Vector2f(
			GetPadding().x + GetPadding().z,
			GetPadding().y + GetPadding().w
		));
	}
}

void TextView::SetFontSize(USize32 size) {
	fontSize = size;
	m_resizedFontSize = size;
	font->LoadSizedFont(size);
}

void TextView::SetFont(ASSETS::AssetRef<ASSETS::Font> font) {
	this->font = font;
}

void TextView::SetText(const std::string& text) {
	this->text = text;
}

const ASSETS::Font* TextView::GetFont() const {
	return font.GetAsset();
}

USize32 TextView::GetFontSize() const {
	return fontSize;
}

std::string_view TextView::GetText() const {
	return text;
}

void TextView::Render(ISdfRenderer2D* renderer) const {
	IElement::Render(renderer);

	if (!font.GetAsset()) {
		return;
	}

	Vector2f globalPosition = GetContentTopLeftPosition();
	globalPosition = globalPosition.ToVector2i().ToVector2f();
	globalPosition.y += font->GetExistingInstance(m_resizedFontSize).characters.at('A').bearing.y;

	SdfStringInfo info{};
	info.text = text;
	info.font = &font->GetExistingInstance(m_resizedFontSize);
	info.transform = Transform2D(EMPTY_GAME_OBJECT);
	info.transform.SetPosition(globalPosition);

	renderer->Draw(info);
}

void TextView::SetSize(Vector2f size) {
	const auto previousSize = GetSize();

	if (KeepsRelativeSize()) {
		const auto contentSize = size - GetPadding2D();
		const auto previousContentSize = previousSize - GetPadding2D();

		const float ratioY = contentSize.y / previousContentSize.y;

		m_resizedFontSize = static_cast<float>(m_resizedFontSize) * ratioY;

		font->LoadSizedFont(m_resizedFontSize);

		if (m_textSize) {
			AdjustSizeToText(); // Aquí se hace el m_size = size;
		}
	}
	else {
		m_size = size;
		IElement::OnSizeChanged(previousSize);
	}
}
