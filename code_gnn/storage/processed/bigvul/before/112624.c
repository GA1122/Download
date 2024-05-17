void Document::webkitWillExitFullScreenForElement(Element*)
{
    if (!m_fullScreenElement)
        return;

    if (!attached() || inPageCache())
        return;

    m_fullScreenElement->willStopBeingFullscreenElement();
}
