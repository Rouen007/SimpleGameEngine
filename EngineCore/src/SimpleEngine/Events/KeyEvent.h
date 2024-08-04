#pragma once

#include "Event.h"


namespace SE
{
	class SENGINE_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keyCode)
			:m_KeyCode(keyCode)
		{}
		int m_KeyCode;
	};

	class SENGINE_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatedCount)
			: KeyEvent(keyCode)
			, m_RepeatedCount(repeatedCount)

		{}

		inline int GetRepeatedCount() const { return m_RepeatedCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << ": " << m_RepeatedCount;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);

	private:
		int m_RepeatedCount;
	};

	class SENGINE_API KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(int keyCode)
			: KeyEvent(keyCode)
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);

	};

	
}