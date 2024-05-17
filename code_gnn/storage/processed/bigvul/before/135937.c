static void dispatchChildRemovalEvents(Node& child)
{
    if (child.isInShadowTree()) {
        InspectorInstrumentation::willRemoveDOMNode(&child);
        return;
    }

    ASSERT(!EventDispatchForbiddenScope::isEventDispatchForbidden());

    InspectorInstrumentation::willRemoveDOMNode(&child);

    RefPtrWillBeRawPtr<Node> c(child);
    RefPtrWillBeRawPtr<Document> document(child.document());

    if (c->parentNode() && document->hasListenerType(Document::DOMNODEREMOVED_LISTENER)) {
        NodeChildRemovalTracker scope(child);
        c->dispatchScopedEvent(MutationEvent::create(EventTypeNames::DOMNodeRemoved, true, c->parentNode()));
    }

    if (c->inDocument() && document->hasListenerType(Document::DOMNODEREMOVEDFROMDOCUMENT_LISTENER)) {
        NodeChildRemovalTracker scope(child);
        for (; c; c = NodeTraversal::next(*c, &child))
            c->dispatchScopedEvent(MutationEvent::create(EventTypeNames::DOMNodeRemovedFromDocument, false));
    }
}
