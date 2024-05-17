void ReadableStreamReader::error()
{
    ASSERT(isActive());
    m_closed->reject(m_stream->storedException());
}
