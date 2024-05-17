    void append(uint8_t b)
    {
        ensureSpace(1);
        *byteAt(m_position++) = b;
    }
