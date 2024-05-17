void ReadableStreamReader::close()
{
    ASSERT(isActive());
    m_closed->resolve(ToV8UndefinedGenerator());
}
