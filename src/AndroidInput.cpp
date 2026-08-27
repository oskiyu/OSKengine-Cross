#include "AndroidInput.h"

#ifdef OSK_ANDROID

#include <android/input.h>
#include <android_native_app_glue.h>

#include "Vector2.hpp"
#include "OSKengine.h"
#include "Logger.h"

using namespace OSK;
using namespace OSK::IO;


AndroidInput* AndroidInput::m_self = nullptr;


AndroidInput::AndroidInput(android_app* app) : m_app(app) {
	app->onInputEvent = AndroidInput::HandleInputEvent;
	m_self = this;
}

int32_t AndroidInput::HandleInputEvent(android_app* app, AInputEvent* event) {
	if (!AndroidInput::m_self) {
		return 0;
	}

	const auto type = AInputEvent_getType(event);

	switch (type) {

	case AINPUT_EVENT_TYPE_MOTION:
	{
		const auto action = AMotionEvent_getAction(event);
		const auto pointerCount = AMotionEvent_getPointerCount(event);

		switch (action & AMOTION_EVENT_ACTION_MASK) {

		case AMOTION_EVENT_ACTION_POINTER_UP:
		case AMOTION_EVENT_ACTION_UP:
			for (UIndex64 i = 0; i < pointerCount; i++) {
				AndroidInput::m_self->m_inputCache.erase(AndroidInput::m_self->m_inputCache.find(TouchInputUuid(AMotionEvent_getPointerId(event, i))));
			}
			break;


		case AMOTION_EVENT_ACTION_POINTER_DOWN:
		case AMOTION_EVENT_ACTION_DOWN:
		case AMOTION_EVENT_ACTION_MOVE:
			for (UIndex64 i = 0; i < pointerCount; i++) {
				TouchInput input{};
				input.position.x = AMotionEvent_getX(event, i);
				input.position.y = AMotionEvent_getY(event, i);
				input.inputId = TouchInputUuid(AMotionEvent_getPointerId(event, i));

				input.timeSinceInputStart = 0.0f;  // @todo
				input.type = TouchInputType::START; // @todo

				AndroidInput::m_self->m_inputCache[TouchInputUuid(AMotionEvent_getPointerId(event, i))] = input;
			}
			break;
		}
		
	}
		return 1;

	default:
		return 0;

	}
}

void AndroidInput::Update() {
	m_currentFrameInputs = m_inputCache;
}

void AndroidInput::QueryInterface(TInterfaceUuid uuid, void** ptr) const {
	if (uuid == OSK_IUUID(ITouchInput)) {
		*ptr = (ITouchInput*)this;
	}
	else {
		*ptr = nullptr;
	}
}

void AndroidInput::QueryConstInterface(TInterfaceUuid uuid, const void** ptr) const {
	if (uuid == OSK_IUUID(ITouchInput)) {
		*ptr = (const ITouchInput*)this;
	}
	else {
		*ptr = nullptr;
	}
}

#endif
