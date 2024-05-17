Page* Document::page() const
{
    return m_frame ? m_frame->page() : 0;    
}
