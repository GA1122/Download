void ReadableStreamReader::stop()
{
    if (isActive()) {
        m_stream->error(DOMException::create(AbortError, "The frame stops working."));
    }
    ActiveDOMObject::stop();
}
