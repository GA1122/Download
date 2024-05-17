void retrieveResourcesForFrame(LocalFrame* frame,
    const WebVector<WebCString>& supportedSchemes,
    Vector<LocalFrame*>* visitedFrames,
    Vector<LocalFrame*>* framesToVisit,
    Vector<KURL>* frameURLs,
    Vector<KURL>* resourceURLs)
{
    KURL frameURL = frame->loader().documentLoader()->request().url();

    if (!frameURL.isValid())
        return;

    bool isValidScheme = false;
    for (size_t i = 0; i < supportedSchemes.size(); ++i) {
        if (frameURL.protocolIs(static_cast<CString>(supportedSchemes[i]).data())) {
            isValidScheme = true;
            break;
        }
    }
    if (!isValidScheme)
        return;

    if (visitedFrames->contains(frame))
        return;
    visitedFrames->append(frame);
    if (!frameURLs->contains(frameURL))
        frameURLs->append(frameURL);

    RefPtrWillBeRawPtr<HTMLAllCollection> allElements = frame->document()->all();
    for (unsigned i = 0; i < allElements->length(); ++i) {
        Element* element = allElements->item(i);
        retrieveResourcesForElement(element,
                                    visitedFrames, framesToVisit,
                                    frameURLs, resourceURLs);
    }
}
