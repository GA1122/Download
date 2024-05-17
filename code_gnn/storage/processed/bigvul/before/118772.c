void ContainerNode::parserRemoveChild(Node* oldChild)
{
    ASSERT(oldChild);
    ASSERT(oldChild->parentNode() == this);
    ASSERT(!oldChild->isDocumentFragment());

    Node* prev = oldChild->previousSibling();
    Node* next = oldChild->nextSibling();

    oldChild->updateAncestorConnectedSubframeCountForRemoval();

    ChildListMutationScope(this).willRemoveChild(oldChild);
    oldChild->notifyMutationObserversNodeWillDetach();

    removeBetween(prev, next, oldChild);

    childrenChanged(true, prev, next, -1);
    ChildNodeRemovalNotifier(this).notify(oldChild);
}
