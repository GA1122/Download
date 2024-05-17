    Reader(const uint8_t* buffer, int length, const WebBlobInfoArray* blobInfo, BlobDataHandleMap& blobDataHandles, ScriptState* scriptState)
        : m_scriptState(scriptState)
        , m_buffer(buffer)
        , m_length(length)
        , m_position(0)
        , m_version(0)
        , m_blobInfo(blobInfo)
        , m_blobDataHandles(blobDataHandles)
    {
        ASSERT(!(reinterpret_cast<size_t>(buffer) & 1));
        ASSERT(length >= 0);
    }
