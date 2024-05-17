    void writeUCharString(v8::Handle<v8::String>& string)
    {
        int length = string->Length();
        ASSERT(length >= 0);

        int size = length * sizeof(UChar);
        int bytes = bytesNeededToWireEncode(static_cast<uint32_t>(size));
        if ((m_position + 1 + bytes) & 1)
            append(PaddingTag);

        append(StringUCharTag);
        doWriteUint32(static_cast<uint32_t>(size));
        ensureSpace(size);

        ASSERT(!(m_position & 1));
        uint16_t* buffer = reinterpret_cast<uint16_t*>(byteAt(m_position));
        string->Write(buffer, 0, length, v8StringWriteOptions());
        m_position += size;
    }
