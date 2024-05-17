void PageSerializer::serializeFrame(LocalFrame* frame)
{
    ASSERT(frame->document());
    Document& document = *frame->document();
    KURL url = document.url();
    if (!url.isValid() || url.protocolIsAbout()) {
        url = urlForBlankFrame(frame);
    }

    if (m_resourceURLs.contains(url)) {
        return;
    }

    if (document.isImageDocument()) {
        ImageDocument& imageDocument = toImageDocument(document);
        addImageToResources(imageDocument.cachedImage(), imageDocument.imageElement()->layoutObject(), url);
        return;
    }

    WillBeHeapVector<RawPtrWillBeMember<Node>> serializedNodes;
    String text;
    if (!m_rewriteURLs.isEmpty()) {
        LinkChangeSerializerMarkupAccumulator accumulator(this, document, serializedNodes, m_rewriteURLs, m_rewriteFolder);
        text = serializeNodes<EditingStrategy>(accumulator, document, IncludeNode);
    } else {
        SerializerMarkupAccumulator accumulator(this, document, serializedNodes);
        text = serializeNodes<EditingStrategy>(accumulator, document, IncludeNode);
    }

    CString frameHTML = document.encoding().normalizeAndEncode(text, WTF::EntitiesForUnencodables);
    m_resources->append(SerializedResource(url, document.suggestedMIMEType(), SharedBuffer::create(frameHTML.data(), frameHTML.length())));
    m_resourceURLs.add(url);

    for (Node* node: serializedNodes) {
        ASSERT(node);
        if (!node->isElementNode())
            continue;

        Element& element = toElement(*node);
        if (element.isStyledElement()) {
            retrieveResourcesForProperties(element.inlineStyle(), document);
            retrieveResourcesForProperties(element.presentationAttributeStyle(), document);
        }

        if (isHTMLImageElement(element)) {
            HTMLImageElement& imageElement = toHTMLImageElement(element);
            KURL url = document.completeURL(imageElement.getAttribute(HTMLNames::srcAttr));
            ImageResource* cachedImage = imageElement.cachedImage();
            addImageToResources(cachedImage, imageElement.layoutObject(), url);
        } else if (isHTMLInputElement(element)) {
            HTMLInputElement& inputElement = toHTMLInputElement(element);
            if (inputElement.type() == InputTypeNames::image && inputElement.imageLoader()) {
                KURL url = inputElement.src();
                ImageResource* cachedImage = inputElement.imageLoader()->image();
                addImageToResources(cachedImage, inputElement.layoutObject(), url);
            }
        } else if (isHTMLLinkElement(element)) {
            HTMLLinkElement& linkElement = toHTMLLinkElement(element);
            if (CSSStyleSheet* sheet = linkElement.sheet()) {
                KURL url = document.completeURL(linkElement.getAttribute(HTMLNames::hrefAttr));
                serializeCSSStyleSheet(*sheet, url);
                ASSERT(m_resourceURLs.contains(url));
            }
        } else if (isHTMLStyleElement(element)) {
            HTMLStyleElement& styleElement = toHTMLStyleElement(element);
            if (CSSStyleSheet* sheet = styleElement.sheet())
                serializeCSSStyleSheet(*sheet, KURL());
        }
    }

    for (Frame* childFrame = frame->tree().firstChild(); childFrame; childFrame = childFrame->tree().nextSibling()) {
        if (childFrame->isLocalFrame())
            serializeFrame(toLocalFrame(childFrame));
    }
}
