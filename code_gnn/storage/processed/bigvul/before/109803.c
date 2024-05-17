void Document::setCSSTarget(Element* n)
{
    if (m_cssTarget)
        m_cssTarget->didAffectSelector(AffectedSelectorTarget);
    m_cssTarget = n;
    if (n)
        n->didAffectSelector(AffectedSelectorTarget);
}
