void Document::nodeChildrenWillBeRemoved(ContainerNode& container)
{
    EventDispatchForbiddenScope assertNoEventDispatch;
    for (Range* range : m_ranges)
        range->nodeChildrenWillBeRemoved(container);

    for (NodeIterator* ni : m_nodeIterators) {
        for (Node& n : NodeTraversal::childrenOf(container))
            ni->nodeWillBeRemoved(n);
    }

    if (LocalFrame* frame = this->frame()) {
        for (Node& n : NodeTraversal::childrenOf(container)) {
            frame->eventHandler().nodeWillBeRemoved(n);
            frame->selection().nodeWillBeRemoved(n);
            frame->page()->dragCaretController().nodeWillBeRemoved(n);
        }
    }
}
