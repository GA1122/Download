void Document::setCSSTarget(Element* newTarget)
{
    if (m_cssTarget)
        m_cssTarget->pseudoStateChanged(CSSSelector::PseudoTarget);
    m_cssTarget = newTarget;
    if (m_cssTarget)
        m_cssTarget->pseudoStateChanged(CSSSelector::PseudoTarget);
}
