#ifndef __Event_H__
#define __Event_H__

enum EventTypes
{
    EventType_Input,
    EventType_NotSet,
};

class Event
{
public:
    EventTypes m_EventType;

    Event()
    {
        m_EventType = EventType_NotSet;
    }
};

#endif //__Event_H__