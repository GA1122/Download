    void writeOneByteString(v8::Handle<v8::String>& string)
    {
        int stringLength = string->Length();
        int utf8Length = string->Utf8Length();
        ASSERT(stringLength >= 0 && utf8Length >= 0);

        append(StringTag);
        doWriteUint32(static_cast<uint32_t>(utf8Length));
        ensureSpace(utf8Length);

        if (stringLength == utf8Length) {
            string->WriteOneByte(byteAt(m_position), 0, utf8Length, v8StringWriteOptions());
        } else {
            char* buffer = reinterpret_cast<char*>(byteAt(m_position));
            string->WriteUtf8(buffer, utf8Length, 0, v8StringWriteOptions());
        }
        m_position += utf8Length;
    }
