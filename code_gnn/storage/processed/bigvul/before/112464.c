void Document::fullScreenElementRemoved()
{
    m_fullScreenElement->setContainsFullScreenElementOnAncestorsCrossingFrameBoundaries(false);
    webkitCancelFullScreen();
}
