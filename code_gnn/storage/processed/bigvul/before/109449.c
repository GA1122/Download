String FileReaderLoader::stringResult()
{
    ASSERT(m_readType != ReadAsArrayBuffer && m_readType != ReadAsBlob);

    if (!m_rawData || m_errorCode)
        return m_stringResult;

    if (m_isRawDataConverted)
        return m_stringResult;

    switch (m_readType) {
    case ReadAsArrayBuffer:
        break;
    case ReadAsBinaryString:
        m_stringResult = String(static_cast<const char*>(m_rawData->data()), m_bytesLoaded);
        break;
    case ReadAsText:
        convertToText();
        break;
    case ReadAsDataURL:
        if (isCompleted())
            convertToDataURL();
        break;
    default:
        ASSERT_NOT_REACHED();
    }
    
    return m_stringResult;
}
