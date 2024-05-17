void StyleResolver::pushParentElement(Element& parent)
{
    const ContainerNode* parentsParent = parent.parentOrShadowHostElement();

    if (!parentsParent || m_selectorFilter.parentStackIsEmpty())
        m_selectorFilter.setupParentStack(parent);
    else
        m_selectorFilter.pushParent(parent);

    m_styleTree.pushStyleCache(parent, parent.parentOrShadowHostNode());
}
