void ReadableStream::setReader(ReadableStreamReader* reader)
{
    ASSERT((reader && !m_reader) || (!reader && m_reader));
    m_reader = reader;
}
