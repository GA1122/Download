void StyleResolver::resetAtHostRules(const ShadowRoot* shadowRoot)
{
    if (!shadowRoot)
        return;

    const ContainerNode* shadowHost = shadowRoot->shadowHost();
    ASSERT(shadowHost);
    ScopedStyleResolver* resolver = m_styleTree.lookupScopedStyleResolverFor(shadowHost);
    if (!resolver)
        return;

    resolver->resetAtHostRules(shadowRoot);
    if (!resolver->hasOnlyEmptyRuleSets())
        return;

    m_styleTree.remove(shadowHost);
}
