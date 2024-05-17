PassRefPtr<Document> XSLTProcessor::createDocumentFromSource(const String& sourceString,
    const String& sourceEncoding, const String& sourceMIMEType, Node* sourceNode, Frame* frame)
{
    RefPtr<Document> ownerDocument = sourceNode->document();
    bool sourceIsDocument = (sourceNode == ownerDocument.get());
    String documentSource = sourceString;

    RefPtr<Document> result;
    if (sourceMIMEType == "text/plain") {
        result = Document::create(frame, sourceIsDocument ? ownerDocument->url() : KURL());
        transformTextStringToXHTMLDocumentString(documentSource);
    } else
        result = DOMImplementation::createDocument(sourceMIMEType, frame, sourceIsDocument ? ownerDocument->url() : KURL(), false);

    if (frame) {
        if (FrameView* view = frame->view())
            view->clear();

        if (Document* oldDocument = frame->document()) {
            result->setTransformSourceDocument(oldDocument);
            result->setSecurityOrigin(oldDocument->securityOrigin());
            result->setCookieURL(oldDocument->cookieURL());
            result->setFirstPartyForCookies(oldDocument->firstPartyForCookies());
            result->contentSecurityPolicy()->copyStateFrom(oldDocument->contentSecurityPolicy());
        }

        frame->setDocument(result);
    }

    RefPtr<TextResourceDecoder> decoder = TextResourceDecoder::create(sourceMIMEType);
    decoder->setEncoding(sourceEncoding.isEmpty() ? UTF8Encoding() : TextEncoding(sourceEncoding), TextResourceDecoder::EncodingFromXMLHeader);
    result->setDecoder(decoder.release());

    result->setContent(documentSource);

     return result.release();
 }
