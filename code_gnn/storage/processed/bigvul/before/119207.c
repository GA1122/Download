void XMLHttpRequest::sendBytesData(const void* data, size_t length, ExceptionState& es)
{
    if (!initSend(es))
        return;

    if (areMethodAndURLValidForSend()) {
        m_requestEntityBody = FormData::create(data, length);
        if (m_upload)
            m_requestEntityBody->setAlwaysStream(true);
    }

    createRequest(es);
}
