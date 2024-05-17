void InspectorPageAgent::frameStoppedLoading(Frame* frame)
{
    m_frontend->frameStoppedLoading(frameId(frame));
}
