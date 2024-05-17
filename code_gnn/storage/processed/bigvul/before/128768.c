bool ReadableStreamReader::hasPendingActivity() const
{
    return isActive() && m_stream->stateInternal() == ReadableStream::Readable;
}
