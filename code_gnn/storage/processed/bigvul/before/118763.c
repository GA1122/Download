static void dispatchChildRemovalEvents(Node* child)
{
    if (child->isInShadowTree()) {
        InspectorInstrumentation::willRemoveDOMNode(&child->document(), child);
        return;
    }

    ASSERT(!NoEventDispatchAssertion::isEventDispatchForbidden());

    InspectorInstrumentation::willRemoveDOMNode(&child->document(), child);

    RefPtr<Node> c = child;
    RefPtr<Document> document = &child->document();

    if (c->parentNode() && document->hasListenerType(Document::DOMNODEREMOVED_LISTENER)) {
        NodeChildRemovalTracker scope(child);
        c->dispatchScopedEvent(MutationEvent::create(eventNames().DOMNodeRemovedEvent, true, c->parentNode()));
    }

    if (c->inDocument() && document->hasListenerType(Document::DOMNODEREMOVEDFROMDOCUMENT_LISTENER)) {
        NodeChildRemovalTracker scope(child);
        for (; c; c = NodeTraversal::next(c.get(), child))
            c->dispatchScopedEvent(MutationEvent::create(eventNames().DOMNodeRemovedFromDocumentEvent, false));
    }
}
