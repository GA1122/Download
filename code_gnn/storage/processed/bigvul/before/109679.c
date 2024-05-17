PassRefPtr<FontFaceSet> Document::fonts()
{
    if (!m_fonts)
        m_fonts = FontFaceSet::create(this);
    return m_fonts;
}
