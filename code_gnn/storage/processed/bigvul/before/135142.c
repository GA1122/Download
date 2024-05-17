static void assertLayoutTreeUpdated(Node& root)
{
    for (Node& node : NodeTraversal::inclusiveDescendantsOf(root)) {
        if (!node.isElementNode()
            && !node.isTextNode()
            && !node.isShadowRoot()
            && !node.isDocumentNode())
            continue;
        ASSERT(!node.needsStyleRecalc());
        ASSERT(!node.childNeedsStyleRecalc());
        ASSERT(!node.childNeedsDistributionRecalc());
        ASSERT(!node.needsStyleInvalidation());
        ASSERT(!node.childNeedsStyleInvalidation());
        for (ShadowRoot* shadowRoot = node.youngestShadowRoot(); shadowRoot; shadowRoot = shadowRoot->olderShadowRoot())
            assertLayoutTreeUpdated(*shadowRoot);
    }
}
