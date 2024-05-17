    bool readArrayBufferViewSubTag(ArrayBufferViewSubTag* tag)
    {
        if (m_position >= m_length)
            return false;
        *tag = static_cast<ArrayBufferViewSubTag>(m_buffer[m_position++]);
        return true;
    }
