    void ensureSpace(unsigned extra)
    {
        COMPILE_ASSERT(sizeof(BufferValueType) == 2, BufferValueTypeIsTwoBytes);
        m_buffer.resize((m_position + extra + 1) / sizeof(BufferValueType));  
    }
