void Document::setAnimatingFullScreen(bool flag)
{
    if (m_isAnimatingFullScreen == flag)
        return;
    m_isAnimatingFullScreen = flag;

    if (m_fullScreenElement && m_fullScreenElement->isDescendantOf(this)) {
        m_fullScreenElement->setNeedsStyleRecalc();
        scheduleForcedStyleRecalc();
    }
}
