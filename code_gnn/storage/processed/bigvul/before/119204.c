void XMLHttpRequest::send(DOMFormData* body, ExceptionState& es)
{
    if (!initSend(es))
        return;

    if (areMethodAndURLValidForSend()) {
        m_requestEntityBody = FormData::createMultiPart(*(static_cast<FormDataList*>(body)), body->encoding(), document());

        String contentType = getRequestHeader("Content-Type");
        if (contentType.isEmpty()) {
            contentType = String("multipart/form-data; boundary=") + m_requestEntityBody->boundary().data();
            setRequestHeaderInternal("Content-Type", contentType);
        }
    }

    createRequest(es);
}
