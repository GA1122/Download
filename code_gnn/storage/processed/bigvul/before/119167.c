void XMLHttpRequest::didReceiveData(const char* data, int len)
{
    if (m_error)
        return;

    if (m_state < HEADERS_RECEIVED)
        changeState(HEADERS_RECEIVED);

    bool useDecoder = m_responseTypeCode == ResponseTypeDefault || m_responseTypeCode == ResponseTypeText || m_responseTypeCode == ResponseTypeJSON || m_responseTypeCode == ResponseTypeDocument;

    if (useDecoder && !m_decoder) {
        if (m_responseTypeCode == ResponseTypeJSON)
            m_decoder = TextResourceDecoder::create("application/json", "UTF-8");
        else if (!m_responseEncoding.isEmpty())
            m_decoder = TextResourceDecoder::create("text/plain", m_responseEncoding);
        else if (responseIsXML()) {
            m_decoder = TextResourceDecoder::create("application/xml");
            m_decoder->useLenientXMLDecoding();
        } else if (equalIgnoringCase(responseMIMEType(), "text/html"))
            m_decoder = TextResourceDecoder::create("text/html", "UTF-8");
        else
            m_decoder = TextResourceDecoder::create("text/plain", "UTF-8");
    }

    if (!len)
        return;

    if (len == -1)
        len = strlen(data);

    if (useDecoder) {
        m_responseText = m_responseText.concatenateWith(m_decoder->decode(data, len));
    } else if (m_responseTypeCode == ResponseTypeArrayBuffer || m_responseTypeCode == ResponseTypeBlob) {
        if (!m_binaryResponseBuilder)
            m_binaryResponseBuilder = SharedBuffer::create();
        m_binaryResponseBuilder->append(data, len);
    } else if (m_responseTypeCode == ResponseTypeStream) {
        if (!m_responseStream)
            m_responseStream = Stream::create(responseMIMEType());
        m_responseStream->addData(data, len);
    }

    if (!m_error) {
        long long expectedLength = m_response.expectedContentLength();
        m_receivedLength += len;

        if (m_async) {
            bool lengthComputable = expectedLength > 0 && m_receivedLength <= expectedLength;
            unsigned long long total = lengthComputable ? expectedLength : 0;
            m_progressEventThrottle.dispatchProgressEvent(lengthComputable, m_receivedLength, total);
        }

        if (m_state != LOADING)
            changeState(LOADING);
        else
            callReadyStateChangeListener();
     }
 }
