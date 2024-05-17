void Document::webkitDidExitFullScreenForElement(Element*)
{
    if (!m_fullScreenElement)
        return;

    if (!attached() || inPageCache())
        return;

    m_fullScreenElement->setContainsFullScreenElementOnAncestorsCrossingFrameBoundaries(false);

    m_areKeysEnabledInFullScreen = false;
    
    if (m_fullScreenRenderer)
        m_fullScreenRenderer->unwrapRenderer();

    m_fullScreenElement = 0;
    scheduleForcedStyleRecalc();
    
    Document* exitingDocument = this;
    if (m_fullScreenChangeEventTargetQueue.isEmpty() && m_fullScreenErrorEventTargetQueue.isEmpty())
        exitingDocument = topDocument();
    exitingDocument->m_fullScreenChangeDelayTimer.startOneShot(0);
}
