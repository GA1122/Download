void ReadableStreamReader::releaseLock(ExceptionState& es)
{
    if (!isActive())
        return;
    if (m_stream->hasPendingReads()) {
        es.throwTypeError("The stream has pending read operations.");
        return;
    }

    releaseLock();
}
