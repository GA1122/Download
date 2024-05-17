void StyleResolver::popParentShadowRoot(const ShadowRoot& shadowRoot)
{
    ASSERT(shadowRoot.host());
    m_styleTree.popStyleCache(shadowRoot);
}
