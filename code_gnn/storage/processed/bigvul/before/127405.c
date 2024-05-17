void StyleResolver::resetAuthorStyle(const ContainerNode* scopingNode)
{
    ScopedStyleResolver* resolver = scopingNode ? m_styleTree.lookupScopedStyleResolverFor(scopingNode) : m_styleTree.scopedStyleResolverForDocument();
    if (!resolver)
        return;

    treeBoundaryCrossingRules().reset(scopingNode);

    resolver->resetAuthorStyle();
    if (!scopingNode)
        return;

    if (scopingNode->isInShadowTree())
        resetAtHostRules(scopingNode->containingShadowRoot());

    if (!resolver->hasOnlyEmptyRuleSets())
        return;

    m_styleTree.remove(scopingNode);
}
