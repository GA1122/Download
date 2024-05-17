Document* ScreenOrientation::document() const
{
    if (!m_associatedDOMWindow || !m_associatedDOMWindow->isCurrentlyDisplayedInFrame())
        return 0;
    ASSERT(m_associatedDOMWindow->document());
    return m_associatedDOMWindow->document();
}
