void retrieveResourcesForElement(Element* element,
                                 Vector<LocalFrame*>* visitedFrames,
                                 Vector<LocalFrame*>* framesToVisit,
                                 Vector<KURL>* frameURLs,
                                 Vector<KURL>* resourceURLs)
{
    ASSERT(element);
    if (isHTMLFrameElementBase(*element) || isHTMLObjectElement(*element) || isHTMLEmbedElement(*element)) {
        Frame* frame = toHTMLFrameOwnerElement(element)->contentFrame();
        if (frame && frame->isLocalFrame()) {
            if (!visitedFrames->contains(toLocalFrame(frame)))
                framesToVisit->append(toLocalFrame(frame));
            return;
        }
    }

    KURL url = getSubResourceURLFromElement(element);
    if (url.isEmpty() || !url.isValid())
        return;  

    if (!url.protocolIsInHTTPFamily() && !url.isLocalFile())
        return;

    if (!resourceURLs->contains(url))
        resourceURLs->append(url);
}
