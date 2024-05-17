void WebPageSerializerImpl::encodeAndFlushBuffer(
    WebPageSerializerClient::PageSerializationStatus status,
    SerializeDomParam* param,
    FlushOption flushOption)
{
    if (flushOption != ForceFlush && m_dataBuffer.length() <= dataBufferCapacity)
        return;

    String content = m_dataBuffer.toString();
    m_dataBuffer.clear();

    CString encodedContent = param->textEncoding.normalizeAndEncode(content, WTF::EntitiesForUnencodables);

    m_client->didSerializeDataForFrame(param->url,
                                       WebCString(encodedContent.data(), encodedContent.length()),
                                        status);
 }
