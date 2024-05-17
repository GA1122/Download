Document* Document::parentDocument() const
{
    if (!m_frame)
        return 0;
    Frame* parent = m_frame->tree()->parent();
    if (!parent)
        return 0;
    return parent->document();
}
