    bool readUCharString(v8::Handle<v8::Value>* value)
    {
        uint32_t length;
        if (!doReadUint32(&length) || (length & 1))
            return false;
        if (m_position + length > m_length)
            return false;
        ASSERT(!(m_position & 1));
        *value = v8::String::NewFromTwoByte(isolate(), reinterpret_cast<const uint16_t*>(m_buffer + m_position), v8::String::kNormalString, length / sizeof(UChar));
        m_position += length;
        return true;
    }
