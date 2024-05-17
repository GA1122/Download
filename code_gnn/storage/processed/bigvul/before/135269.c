DOMImplementation& Document::implementation()
{
    if (!m_implementation)
        m_implementation = DOMImplementation::create(*this);
    return *m_implementation;
}
