Document* XMLHttpRequest::responseXML(ExceptionState& es)
{
    if (m_responseTypeCode != ResponseTypeDefault && m_responseTypeCode != ResponseTypeDocument) {
        es.throwDOMException(InvalidStateError, ExceptionMessages::failedToGet("responseXML", "XMLHttpRequest", "the value is only accessible if the object's 'responseType' is '' or 'document' (was '" + responseType() + "')."));
        return 0;
    }

    if (m_error || m_state != DONE)
        return 0;

    if (!m_createdDocument) {
        bool isHTML = equalIgnoringCase(responseMIMEType(), "text/html");

        if ((m_response.isHTTP() && !responseIsXML() && !isHTML)
            || (isHTML && m_responseTypeCode == ResponseTypeDefault)
            || scriptExecutionContext()->isWorkerGlobalScope()) {
            m_responseDocument = 0;
        } else {
            DocumentInit init = DocumentInit::fromContext(document()->contextDocument(), m_url);
            if (isHTML)
                m_responseDocument = HTMLDocument::create(init);
            else
                m_responseDocument = Document::create(init);
            m_responseDocument->setContent(m_responseText.flattenToString());
            m_responseDocument->setSecurityOrigin(securityOrigin());
            m_responseDocument->setContextFeatures(document()->contextFeatures());
            if (!m_responseDocument->wellFormed())
                m_responseDocument = 0;
        }
        m_createdDocument = true;
    }

    return m_responseDocument.get();
}
