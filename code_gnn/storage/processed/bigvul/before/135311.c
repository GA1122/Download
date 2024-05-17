void Document::nodeWillBeRemoved(Node& n)
{
    for (NodeIterator* ni : m_nodeIterators)
        ni->nodeWillBeRemoved(n);

    for (Range* range : m_ranges)
        range->nodeWillBeRemoved(n);

    if (LocalFrame* frame = this->frame()) {
        frame->eventHandler().nodeWillBeRemoved(n);
        frame->selection().nodeWillBeRemoved(n);
        frame->page()->dragCaretController().nodeWillBeRemoved(n);
    }
}
