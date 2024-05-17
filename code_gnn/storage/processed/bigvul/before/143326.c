Settings* Frame::settings() const
{
    if (m_host)
        return &m_host->settings();
    return nullptr;
}
