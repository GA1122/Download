DEFINE_TRACE(ReadableStreamReader)
{
    visitor->trace(m_stream);
    visitor->trace(m_closed);
    ActiveDOMObject::trace(visitor);
}
