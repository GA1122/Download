Document* DocumentLoader::document() const
{
    if (m_frame && m_frame->loader()->documentLoader() == this)
        return m_frame->document();
    return 0;
}
