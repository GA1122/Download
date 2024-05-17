void ContainerNode::willRemoveChildren()
{
    NodeVector children;
    getChildNodes(*this, children);

    ChildListMutationScope mutation(*this);
    for (const auto& node : children) {
        ASSERT(node);
        Node& child = *node;
        mutation.willRemoveChild(child);
        child.notifyMutationObserversNodeWillDetach();
        dispatchChildRemovalEvents(child);
    }

    ChildFrameDisconnector(*this).disconnect(ChildFrameDisconnector::DescendantsOnly);
}
