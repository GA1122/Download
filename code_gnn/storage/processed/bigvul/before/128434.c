static inline RenderObject* rendererForScrollbar(RenderObject& renderer)
{
    if (Node* node = renderer.node()) {
        if (ShadowRoot* shadowRoot = node->containingShadowRoot()) {
            if (shadowRoot->type() == ShadowRoot::UserAgentShadowRoot)
                return shadowRoot->host()->renderer();
        }
    }

    return &renderer;
}
