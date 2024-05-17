void Range::formatForDebugger(char* buffer, unsigned length) const
{
    String result;
    String s;
    
    if (!m_start.container() || !m_end.container())
        result = "<empty>";
    else {
        char s[FormatBufferSize];
        result += "from offset ";
        result += String::number(m_start.offset());
        result += " of ";
        m_start.container()->formatForDebugger(s, FormatBufferSize);
        result += s;
        result += " to offset ";
        result += String::number(m_end.offset());
        result += " of ";
        m_end.container()->formatForDebugger(s, FormatBufferSize);
        result += s;
    }
          
    strncpy(buffer, result.utf8().data(), length - 1);
}
