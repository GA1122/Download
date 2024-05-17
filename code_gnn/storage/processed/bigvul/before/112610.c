void Document::updateTitle(const StringWithDirection& title)
{
    if (m_rawTitle == title)
        return;

    m_rawTitle = title;

    if (m_rawTitle.string().isEmpty())
        m_title = StringWithDirection();
    else {
        if (m_rawTitle.string().is8Bit())
            m_title = canonicalizedTitle<LChar>(this, m_rawTitle);
        else
            m_title = canonicalizedTitle<UChar>(this, m_rawTitle);
    }
    if (Frame* f = frame())
        f->loader()->setTitle(m_title);
}
