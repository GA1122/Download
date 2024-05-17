void XMLHttpRequest::send(const String& body, ExceptionState& es)
{
    if (!initSend(es))
        return;

    if (!body.isNull() && areMethodAndURLValidForSend()) {
        String contentType = getRequestHeader("Content-Type");
        if (contentType.isEmpty()) {
            setRequestHeaderInternal("Content-Type", "text/plain;charset=UTF-8");
        } else {
            replaceCharsetInMediaType(contentType, "UTF-8");
            m_requestHeaders.set("Content-Type", contentType);
        }

        m_requestEntityBody = FormData::create(UTF8Encoding().encode(body, WTF::EntitiesForUnencodables));
        if (m_upload)
            m_requestEntityBody->setAlwaysStream(true);
    }

    createRequest(es);
}
