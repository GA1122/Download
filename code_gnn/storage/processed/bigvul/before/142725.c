String FrameLoader::userAgent() const
{
    String userAgent = client()->userAgent();
    InspectorInstrumentation::applyUserAgentOverride(m_frame, &userAgent);
    return userAgent;
}
