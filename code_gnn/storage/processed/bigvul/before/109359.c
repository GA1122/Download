    virtual void didFinishLoading(unsigned long  , double  )
    {
        if (m_decoder)
            m_responseText = m_responseText.concatenateWith(m_decoder->flush());
        m_callback->sendSuccess(m_statusCode, buildObjectForHeaders(m_responseHeaders), m_responseText.flattenToString());
        dispose();
    }
