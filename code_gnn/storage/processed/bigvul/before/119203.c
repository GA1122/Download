void XMLHttpRequest::send(Blob* body, ExceptionState& es)
{
    if (!initSend(es))
        return;

    if (areMethodAndURLValidForSend()) {
        const String& contentType = getRequestHeader("Content-Type");
        if (contentType.isEmpty()) {
            const String& blobType = body->type();
            if (!blobType.isEmpty() && isValidContentType(blobType))
                setRequestHeaderInternal("Content-Type", blobType);
            else {
                setRequestHeaderInternal("Content-Type", "");
            }
        }

        m_requestEntityBody = FormData::create();
        if (body->isFile())
            m_requestEntityBody->appendFile(toFile(body)->path());
        else
            m_requestEntityBody->appendBlob(body->url());
    }

    createRequest(es);
}
