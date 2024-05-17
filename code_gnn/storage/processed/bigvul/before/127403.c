void StyleResolver::pushParentShadowRoot(const ShadowRoot& shadowRoot)
{
    ASSERT(shadowRoot.host());
    m_styleTree.pushStyleCache(shadowRoot, shadowRoot.host());
}
