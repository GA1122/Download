    bool getNextLine(std::string* line)
    {
        line->clear();
        if (m_index >= m_text.length())
            return false;

        size_t endOfLineIndex = m_text.find("\r\n", m_index);
        if (endOfLineIndex == std::string::npos) {
            *line = m_text.substr(m_index);
            m_index = m_text.length();
        } else {
            *line = m_text.substr(m_index, endOfLineIndex - m_index);
            m_index = endOfLineIndex + 2;
        }
        return true;
    }
