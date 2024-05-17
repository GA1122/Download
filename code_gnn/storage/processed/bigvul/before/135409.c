TextAutosizer* Document::textAutosizer()
{
    if (!m_textAutosizer)
        m_textAutosizer = TextAutosizer::create(this);
    return m_textAutosizer.get();
}
