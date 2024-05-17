ArrayBuffer* XMLHttpRequest::responseArrayBuffer()
{
    ASSERT(m_responseTypeCode == ResponseTypeArrayBuffer);

    if (m_error || m_state != DONE)
        return 0;

    if (!m_responseArrayBuffer.get()) {
        if (m_binaryResponseBuilder.get() && m_binaryResponseBuilder->size() > 0) {
            m_responseArrayBuffer = m_binaryResponseBuilder->getAsArrayBuffer();
            m_binaryResponseBuilder.clear();
        } else {
            m_responseArrayBuffer = ArrayBuffer::create(static_cast<void*>(0), 0);
        }
    }

    return m_responseArrayBuffer.get();
}
