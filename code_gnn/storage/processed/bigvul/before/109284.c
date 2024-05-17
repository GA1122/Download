void InspectorPageAgent::didLayout(RenderObject*)
{
    if (!m_enabled)
        return;
    m_overlay->update();
}
