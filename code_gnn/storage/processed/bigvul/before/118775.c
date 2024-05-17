void ContainerNode::removeChild(Node* oldChild, ExceptionState& es)
{
    ASSERT(refCount() || parentOrShadowHostNode());

    RefPtr<Node> protect(this);

    if (!oldChild || oldChild->parentNode() != this) {
        es.throwUninformativeAndGenericDOMException(NotFoundError);
        return;
    }

    RefPtr<Node> child = oldChild;

    document().removeFocusedElementOfSubtree(child.get());

    if (FullscreenElementStack* fullscreen = FullscreenElementStack::fromIfExists(&document()))
        fullscreen->removeFullScreenElementOfSubtree(child.get());

    if (child->parentNode() != this) {
        es.throwUninformativeAndGenericDOMException(NotFoundError);
        return;
    }

    willRemoveChild(child.get());

    if (child->parentNode() != this) {
        es.throwUninformativeAndGenericDOMException(NotFoundError);
        return;
    }

    {
        RenderWidget::UpdateSuspendScope suspendWidgetHierarchyUpdates;

        Node* prev = child->previousSibling();
        Node* next = child->nextSibling();
        removeBetween(prev, next, child.get());
        childrenChanged(false, prev, next, -1);
        ChildNodeRemovalNotifier(this).notify(child.get());
    }
    dispatchSubtreeModifiedEvent();
}
