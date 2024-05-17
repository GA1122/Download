WordAwareIterator::WordAwareIterator(const Range* r)
    : m_previousText(0)
    , m_didLookAhead(true)  
    , m_textIterator(r)
{
    advance();  
}
