void Document::didSplitTextNode(Text& oldNode)
{
    for (Range* range : m_ranges)
        range->didSplitTextNode(oldNode);

    if (m_frame)
        m_frame->selection().didSplitTextNode(oldNode);

}
