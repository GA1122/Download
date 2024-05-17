String WebFrame::source() const 
{
    if (!m_coreFrame)
        return String();
    Document* document = m_coreFrame->document();
    if (!document)
        return String();
    TextResourceDecoder* decoder = document->decoder();
    if (!decoder)
        return String();
    DocumentLoader* documentLoader = m_coreFrame->loader()->activeDocumentLoader();
    if (!documentLoader)
        return String();
    RefPtr<SharedBuffer> mainResourceData = documentLoader->mainResourceData();
    if (!mainResourceData)
        return String();
    return decoder->encoding().decode(mainResourceData->data(), mainResourceData->size());
}
