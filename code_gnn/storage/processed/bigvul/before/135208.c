void Document::didRemoveText(Node* text, unsigned offset, unsigned length)
{
    for (Range* range : m_ranges)
        range->didRemoveText(text, offset, length);

    m_markers->removeMarkers(text, offset, length);
    m_markers->shiftMarkers(text, offset + length, 0 - length);
}
