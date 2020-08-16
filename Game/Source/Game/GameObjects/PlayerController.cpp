#include "GamePCH.h"

PlayerController::PlayerController()
{
    Clear();
}

PlayerController::~PlayerController()
{
}

void PlayerController::Clear()
{
    m_ButtonFlags = 0;
}

void PlayerController::SetButtonPressed(PlayerControllerButtons button)
{
    m_ButtonFlags |= button;
}

void PlayerController::SetButtonReleased(PlayerControllerButtons button)
{
    m_ButtonFlags &= ~button;
}

bool PlayerController::IsButtonPressed(PlayerControllerButtons button)
{
    if( m_ButtonFlags & button )
        return true;

    return false;
}

void PlayerController::OnEvent(Event* pEvent)
{
    if( pEvent->m_EventType == EventType_Input )
    {
        InputEvent* pInputEvent = (InputEvent*)pEvent;

        if( pInputEvent->m_InputDeviceType == InputDeviceType_Keyboard )
        {
            if( pInputEvent->m_InputState == InputState_Pressed )
            {
                if( pInputEvent->m_ID == VK_UP || pInputEvent->m_ID == 'W' )
                    SetButtonPressed( PCB_Up );
                if( pInputEvent->m_ID == VK_DOWN || pInputEvent->m_ID == 'S' )
                    SetButtonPressed( PCB_Down );
                if( pInputEvent->m_ID == VK_LEFT || pInputEvent->m_ID == 'A' )
                    SetButtonPressed( PCB_Left );
                if( pInputEvent->m_ID == VK_RIGHT || pInputEvent->m_ID == 'D' )
                    SetButtonPressed( PCB_Right );
            }

            if( pInputEvent->m_InputState == InputState_Released )
            {
                if( pInputEvent->m_ID == VK_UP || pInputEvent->m_ID == 'W' )
                    SetButtonReleased( PCB_Up );
                if( pInputEvent->m_ID == VK_DOWN || pInputEvent->m_ID == 'S' )
                    SetButtonReleased( PCB_Down );
                if( pInputEvent->m_ID == VK_LEFT || pInputEvent->m_ID == 'A' )
                    SetButtonReleased( PCB_Left );
                if( pInputEvent->m_ID == VK_RIGHT || pInputEvent->m_ID == 'D' )
                    SetButtonReleased( PCB_Right );
            }
        }

        if( pInputEvent->m_InputDeviceType == InputDeviceType_Gamepad )
        {
            if( pInputEvent->m_InputState == InputState_Held )
            {
                if( pInputEvent->m_GamepadID == GamepadID_LeftStick )
                {
                    if( pInputEvent->m_Y > 0 )
                        SetButtonReleased( PCB_Up );
                    if( pInputEvent->m_Y < 0 )
                        SetButtonReleased( PCB_Down );
                    if( pInputEvent->m_X < 0 )
                        SetButtonReleased( PCB_Left );
                    if( pInputEvent->m_X > 0 )
                        SetButtonReleased( PCB_Right );

                    if( pInputEvent->m_Y == 0 )
                    {
                        SetButtonReleased( PCB_Up );
                        SetButtonReleased( PCB_Down );
                    }
                    if( pInputEvent->m_X == 0 )
                    {
                        SetButtonReleased( PCB_Left );
                        SetButtonReleased( PCB_Right );
                    }
                }
            }
        }
    }
}