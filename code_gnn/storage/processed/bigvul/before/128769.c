bool ReadableStreamReader::isActive() const
{
    return m_stream->isLockedTo(this);
}
