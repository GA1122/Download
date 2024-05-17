void Document::updateRangesAfterNodeMovedToAnotherDocument(const Node& node)
{
    DCHECK_NE(node.document(), this);
    if (m_ranges.isEmpty())
        return;

    AttachedRangeSet ranges = m_ranges;
    for (Range* range : ranges)
        range->updateOwnerDocumentIfNeeded();
}
