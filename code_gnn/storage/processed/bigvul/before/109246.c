void InspectorOverlay::onTimer(Timer<InspectorOverlay>*)
{
    m_drawViewSize = false;
    update();
}
