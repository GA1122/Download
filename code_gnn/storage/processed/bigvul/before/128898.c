    uint8_t* byteAt(int position)
    {
        return reinterpret_cast<uint8_t*>(m_buffer.data()) + position;
    }
