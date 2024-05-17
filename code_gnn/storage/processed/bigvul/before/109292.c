void InspectorPageAgent::frameAttachedToParent(Frame* frame)
{
    m_frontend->frameAttached(frameId(frame));
}
