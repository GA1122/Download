void DOMPatchSupport::patchDocument(const String& markup)
{
    RefPtr<HTMLDocument> newDocument = HTMLDocument::create(0, KURL());
    RefPtr<DocumentParser> parser = HTMLDocumentParser::create(newDocument.get(), false);
    parser->insert(markup);  
    parser->finish();
    parser->detach();

    OwnPtr<Digest> oldInfo = createDigest(m_document->documentElement(), 0);
    OwnPtr<Digest> newInfo = createDigest(newDocument->documentElement(), &m_unusedNodesMap);

    ExceptionCode ec = 0;
    if (!innerPatchNode(oldInfo.get(), newInfo.get(), ec)) {
        m_document->write(markup);
        m_document->close();
    }
}
