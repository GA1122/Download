String WebFrame::url() const
{
    if (!m_coreFrame)
        return String();

    return m_coreFrame->loader()->url().string();
}
