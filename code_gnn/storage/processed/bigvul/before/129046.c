    void writeStringObject(const char* data, int length)
    {
        ASSERT(length >= 0);
        append(StringObjectTag);
        doWriteString(data, length);
    }
