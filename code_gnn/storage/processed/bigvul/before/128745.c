DEFINE_TRACE(ReadableStream)
{
    visitor->trace(m_source);
    visitor->trace(m_exception);
    visitor->trace(m_reader);
}
