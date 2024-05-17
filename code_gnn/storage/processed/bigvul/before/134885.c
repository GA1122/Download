void WebPageSerializerImpl::saveHTMLContentToBuffer(
    const String& result, SerializeDomParam* param)
{
    m_dataBuffer.append(result);
    encodeAndFlushBuffer(WebPageSerializerClient::CurrentFrameIsNotFinished,
                         param,
                         DoNotForceFlush);
}
