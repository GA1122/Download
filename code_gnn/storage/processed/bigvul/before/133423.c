bool SVGImage::currentFrameHasSingleSecurityOrigin() const
{
    if (!m_page)
        return true;

    LocalFrame* frame = m_page->mainFrame();

    RELEASE_ASSERT(frame->document()->loadEventFinished());

    SVGSVGElement* rootElement = toSVGDocument(frame->document())->rootElement();
    if (!rootElement)
        return true;

    ComposedTreeWalker walker(rootElement);
    while (Node* node = walker.get()) {
        if (node->hasTagName(SVGNames::foreignObjectTag))
            return false;
        if (node->hasTagName(SVGNames::imageTag)) {
            if (!toSVGImageElement(node)->currentFrameHasSingleSecurityOrigin())
                return false;
        } else if (node->hasTagName(SVGNames::feImageTag)) {
            if (!toSVGFEImageElement(node)->currentFrameHasSingleSecurityOrigin())
                return false;
        }
        walker.next();
    }

    return true;
}
