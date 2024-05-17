bool WebPageSerializerImpl::serialize()
{
    bool didSerialization = false;
    KURL mainURL = m_specifiedWebLocalFrameImpl->frame()->document()->url();

    WebLocalFrameImpl* webFrame = m_specifiedWebLocalFrameImpl;
    Document* document = webFrame->frame()->document();
    const KURL& url = document->url();

    if (url.isValid() && m_localLinks.contains(url.string())) {
        didSerialization = true;

        const WTF::TextEncoding& textEncoding = document->encoding().isValid() ? document->encoding() : UTF8Encoding();
        String directoryName = url == mainURL ? m_localDirectoryName : "";

        SerializeDomParam param(url, textEncoding, document, directoryName);

        Element* documentElement = document->documentElement();
        if (documentElement)
            buildContentForNode(documentElement, &param);

        encodeAndFlushBuffer(WebPageSerializerClient::CurrentFrameIsFinished, &param, ForceFlush);
    }

    ASSERT(m_dataBuffer.isEmpty());
    m_client->didSerializeDataForFrame(KURL(), WebCString("", 0), WebPageSerializerClient::AllFramesAreFinished);
    return didSerialization;
}
