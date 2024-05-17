bool WebPageSerializer::retrieveAllResources(WebView* view,
                                             const WebVector<WebCString>& supportedSchemes,
                                             WebVector<WebURL>* resourceURLs,
                                             WebVector<WebURL>* frameURLs) {
    WebLocalFrameImpl* mainFrame = toWebLocalFrameImpl(view->mainFrame());
    if (!mainFrame)
        return false;

    Vector<LocalFrame*> framesToVisit;
    Vector<LocalFrame*> visitedFrames;
    Vector<KURL> frameKURLs;
    Vector<KURL> resourceKURLs;

    framesToVisit.append(mainFrame->frame());
    while (!framesToVisit.isEmpty()) {
        LocalFrame* frame = framesToVisit[0];
        framesToVisit.remove(0);
        retrieveResourcesForFrame(frame, supportedSchemes,
                                  &visitedFrames, &framesToVisit,
                                  &frameKURLs, &resourceKURLs);
    }

    WebVector<WebURL> resultResourceURLs(resourceKURLs.size());
    for (size_t i = 0; i < resourceKURLs.size(); ++i) {
        resultResourceURLs[i] = resourceKURLs[i];
        size_t index = frameKURLs.find(resourceKURLs[i]);
        if (index != kNotFound)
            frameKURLs.remove(index);
    }
    *resourceURLs = resultResourceURLs;
    WebVector<WebURL> resultFrameURLs(frameKURLs.size());
    for (size_t i = 0; i < frameKURLs.size(); ++i)
        resultFrameURLs[i] = frameKURLs[i];
    *frameURLs = resultFrameURLs;

    return true;
}
