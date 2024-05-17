void StyleResolver::popParentElement(Element& parent)
{
    if (m_selectorFilter.parentStackIsConsistent(&parent))
        m_selectorFilter.popParent();

    m_styleTree.popStyleCache(parent);
}
