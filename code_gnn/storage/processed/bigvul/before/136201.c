void SerializerMarkupAccumulator::appendCustomAttributes(StringBuilder& result, const Element& element, Namespaces* namespaces)
{
    if (!element.isFrameOwnerElement())
        return;

    const HTMLFrameOwnerElement& frameOwner = toHTMLFrameOwnerElement(element);
    Frame* frame = frameOwner.contentFrame();
    if (!frame || !frame->isLocalFrame())
        return;

    KURL url = toLocalFrame(frame)->document()->url();
    if (url.isValid() && !url.protocolIsAbout())
        return;

    url = m_serializer->urlForBlankFrame(toLocalFrame(frame));
    appendAttribute(result, element, Attribute(frameOwnerURLAttributeName(frameOwner), AtomicString(url.string())), namespaces);
}
