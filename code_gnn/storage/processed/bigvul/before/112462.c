PassRefPtr<FontLoader> Document::fontloader()
{
    if (!m_fontloader)
        m_fontloader = FontLoader::create(this);
    return m_fontloader;
}
