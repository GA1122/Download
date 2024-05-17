Page* Frame::page() const
{
    if (m_host)
        return &m_host->page();
    return nullptr;
}
