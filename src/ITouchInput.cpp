#include "ITouchInput.h"

using namespace OSK;
using namespace OSK::IO;

DynamicArray<TouchInput> ITouchInput::GetCurrentFrameInputs() const {
	auto output = DynamicArray<TouchInput>::CreateReserved(m_currentFrameInputs.size());

	for (const auto& [uiid, input] : m_currentFrameInputs) {
		output.Insert(input);
	}

	return output;
}
