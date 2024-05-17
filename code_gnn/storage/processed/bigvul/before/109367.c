    virtual void didReceiveResponse(unsigned long identifier, const ResourceResponse& response)
    {
        WTF::TextEncoding textEncoding(response.textEncodingName());
        bool useDetector = false;
        if (!textEncoding.isValid()) {
            textEncoding = UTF8Encoding();
            useDetector = true;
        }
        m_decoder = TextResourceDecoder::create("text/plain", textEncoding, useDetector);
        m_statusCode = response.httpStatusCode();
        m_responseHeaders = response.httpHeaderFields();
    }
