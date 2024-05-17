WebPageSerializerImpl::SerializeDomParam::SerializeDomParam(const KURL& url,
                                                            const WTF::TextEncoding& textEncoding,
                                                            Document* document,
                                                            const String& directoryName)
    : url(url)
    , textEncoding(textEncoding)
    , document(document)
    , directoryName(directoryName)
    , isHTMLDocument(document->isHTMLDocument())
    , haveSeenDocType(false)
    , haveAddedCharsetDeclaration(false)
    , skipMetaElement(nullptr)
    , isInScriptOrStyleTag(false)
    , haveAddedXMLProcessingDirective(false)
    , haveAddedContentsBeforeEnd(false)
{
}
