    bool readString(v8::Handle<v8::Value>* value)
    {
        uint32_t length;
        if (!doReadUint32(&length))
            return false;
        if (m_position + length > m_length)
            return false;
        *value = v8::String::NewFromUtf8(isolate(), reinterpret_cast<const char*>(m_buffer + m_position), v8::String::kNormalString, length);
        m_position += length;
        return true;
    }
