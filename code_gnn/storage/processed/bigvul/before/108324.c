DocumentLoader* Timing::documentLoader() const
{
    if (!m_frame)
        return 0;

    return m_frame->loader()->documentLoader();
}
