void GIFColorMap::buildTable(const unsigned char* data, size_t length)
{
    if (!m_isDefined || !m_table.isEmpty())
        return;

    RELEASE_ASSERT(m_position + m_colors * BYTES_PER_COLORMAP_ENTRY <= length);
    const unsigned char* srcColormap = data + m_position;
    m_table.resize(m_colors);
    for (Table::iterator iter = m_table.begin(); iter != m_table.end(); ++iter) {
        *iter = SkPackARGB32NoCheck(255, srcColormap[0], srcColormap[1], srcColormap[2]);
        srcColormap += BYTES_PER_COLORMAP_ENTRY;
    }
}
