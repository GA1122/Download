Frame::~Frame()
{
    InstanceCounters::decrementCounter(InstanceCounters::FrameCounter);
    ASSERT(!m_owner);
}
