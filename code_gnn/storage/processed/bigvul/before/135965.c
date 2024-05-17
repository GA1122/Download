PassRefPtrWillBeRawPtr<Node> ContainerNode::removeChild(PassRefPtrWillBeRawPtr<Node> oldChild, ExceptionState& exceptionState)
{
#if !ENABLE(OILPAN)
    ASSERT(refCount() || parentOrShadowHostNode());
#endif

    RefPtrWillBeRawPtr<Node> protect(this);

    if (!oldChild || oldChild->parentNode() != this || oldChild->isPseudoElement()) {
        exceptionState.throwDOMException(NotFoundError, "The node to be removed is not a child of this node.");
        return nullptr;
    }

    RefPtrWillBeRawPtr<Node> child = oldChild;

    document().removeFocusedElementOfSubtree(child.get());

    if (child->parentNode() != this) {
        exceptionState.throwDOMException(NotFoundError, "The node to be removed is no longer a child of this node. Perhaps it was moved in a 'blur' event handler?");
        return nullptr;
    }

    willRemoveChild(*child);

    if (child->parentNode() != this) {
        exceptionState.throwDOMException(NotFoundError, "The node to be removed is no longer a child of this node. Perhaps it was moved in response to a mutation?");
        return nullptr;
    }

    {
        HTMLFrameOwnerElement::UpdateSuspendScope suspendWidgetHierarchyUpdates;

        Node* prev = child->previousSibling();
        Node* next = child->nextSibling();
        removeBetween(prev, next, *child);
        notifyNodeRemoved(*child);
        childrenChanged(ChildrenChange::forRemoval(*child, prev, next, ChildrenChangeSourceAPI));
    }
    dispatchSubtreeModifiedEvent();
    return child;
}
