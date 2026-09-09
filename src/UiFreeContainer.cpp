#include "UiFreeContainer.h"

#include "OSKengine.h"
#include "Logger.h"

using namespace OSK;
using namespace OSK::UI;

FreeContainer::FreeContainer(const Vector2f& size) : IContainer(size) {

}

void FreeContainer::EmplaceChild(IElement* child) {
	using EFTraits::HasFlag;
	using enum Anchor;

	auto childOffset = Vector2f::Zero;

	const auto parentSize  = this ->GetSize();
	const auto childAnchor = child->GetAnchor();
	const auto childMargin = child->GetMarging();
	const auto childSize   = child->GetSize();

	// Eje X
	if (HasFlag(childAnchor, LEFT))     childOffset.x = parentSize.x * 0.0f - childSize.x * 0.0f + childMargin.x + GetPadding().x;
	if (HasFlag(childAnchor, CENTER_X)) childOffset.x = parentSize.x * 0.5f - childSize.x * 0.5f;
	if (HasFlag(childAnchor, RIGHT))    childOffset.x = parentSize.x * 1.0f - childSize.x * 1.0f - childMargin.z - GetPadding().z;
	
	// Eje Y
	if (HasFlag(childAnchor, TOP     )) childOffset.y = parentSize.y * 0.0f - childSize.y * 0.0f + childMargin.y + GetPadding().y;
	if (HasFlag(childAnchor, CENTER_Y)) childOffset.y = parentSize.y * 0.5f - childSize.y * 0.5f;
	if (HasFlag(childAnchor, BOTTOM  )) childOffset.y = parentSize.y * 1.0f - childSize.y * 1.0f - childMargin.w - GetPadding().w;

	child->_SetPosition(GetPosition() + childOffset);
}

void FreeContainer::ResetLayout() {

}
