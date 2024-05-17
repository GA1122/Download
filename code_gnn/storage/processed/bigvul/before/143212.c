bool Document::isEffectiveRootScroller(const Element& element) const
{
    return m_rootScrollerController->effectiveRootScroller() == element;
}
