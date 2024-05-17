void Document::updateRangesAfterChildrenChanged(ContainerNode* container)
{
    for (Range* range : m_ranges)
        range->nodeChildrenChanged(container);
}
