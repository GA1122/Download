    virtual void didReceiveData(const char* data, int dataLength)
    {
        if (!dataLength)
            return;

        if (dataLength == -1)
            dataLength = strlen(data);

        m_responseText = m_responseText.concatenateWith(m_decoder->decode(data, dataLength));
    }
