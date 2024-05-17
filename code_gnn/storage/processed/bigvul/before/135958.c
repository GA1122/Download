void ContainerNode::parserRemoveChild(Node& oldChild)
{
    ASSERT(oldChild.parentNode() == this);
    ASSERT(!oldChild.isDocumentFragment());

    if (oldChild.connectedSubframeCount())
        ChildFrameDisconnector(oldChild).disconnect();

    if (oldChild.parentNode() != this)
        return;

    ChildListMutationScope(*this).willRemoveChild(oldChild);
    oldChild.notifyMutationObserversNodeWillDetach();

    Node* prev = oldChild.previousSibling();
    Node* next = oldChild.nextSibling();
    removeBetween(prev, next, oldChild);

    notifyNodeRemoved(oldChild);
    childrenChanged(ChildrenChange::forRemoval(oldChild, prev, next, ChildrenChangeSourceParser));
}
