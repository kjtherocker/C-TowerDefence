#ifndef __InputEvent_H__
#define __InputEvent_H__

enum InputDeviceTypes
{
    InputDeviceType_Keyboard,
    InputDeviceType_Mouse,
    InputDeviceType_Gamepad,
    InputDeviceType_NumTypes,
};

enum InputStates
{
    InputState_Pressed,
    InputState_Released,
    InputState_Moved,
    InputState_Held,
    InputState_NumStates,
};

enum GamepadIDs
{
    GamepadID_LeftStick,
    GamepadID_RightStick,
};

class InputEvent : public Event
{
public:
    InputDeviceTypes m_InputDeviceType;
    InputStates m_InputState;

    union
    {
        int m_ID;
        GamepadIDs m_GamepadID;
    };

    float m_X;
    float m_Y;

    InputEvent(InputDeviceTypes devicetype, InputStates state, int id, float x, float y)
    {
        m_EventType = EventType_Input;

        m_InputDeviceType = devicetype;
        m_InputState = state;
        m_ID = id;
        m_X = x;
        m_Y = y;
    }
};

#endif //__InputEvent_H__