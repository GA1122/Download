void Document::attachRange(Range* range)
{
    DCHECK(!m_ranges.contains(range));
    m_ranges.add(range);
}
