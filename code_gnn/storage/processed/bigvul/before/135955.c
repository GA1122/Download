void ContainerNode::notifyNodeRemoved(Node& root)
{
    ScriptForbiddenScope forbidScript;
    EventDispatchForbiddenScope assertNoEventDispatch;

    for (Node& node : NodeTraversal::inclusiveDescendantsOf(root)) {
        if (!node.isContainerNode() && !node.isInTreeScope())
            continue;
        node.removedFrom(this);
        for (ShadowRoot* shadowRoot = node.youngestShadowRoot(); shadowRoot; shadowRoot = shadowRoot->olderShadowRoot())
            notifyNodeRemoved(*shadowRoot);
    }
}
