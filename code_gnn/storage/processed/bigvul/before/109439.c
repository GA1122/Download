void FileReaderLoader::convertToText()
{
    if (!m_bytesLoaded)
        return;

    StringBuilder builder;
    if (!m_decoder)
        m_decoder = TextResourceDecoder::create("text/plain", m_encoding.isValid() ? m_encoding : UTF8Encoding());
    builder.append(m_decoder->decode(static_cast<const char*>(m_rawData->data()), m_bytesLoaded));

    if (isCompleted())
        builder.append(m_decoder->flush());

    m_stringResult = builder.toString();
}
