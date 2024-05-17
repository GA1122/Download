    String takeWireString()
    {
        COMPILE_ASSERT(sizeof(BufferValueType) == 2, BufferValueTypeIsTwoBytes);
        fillHole();
        String data = String(m_buffer.data(), m_buffer.size());
        data.impl()->truncateAssumingIsolated((m_position + 1) / sizeof(BufferValueType));
        return data;
    }
