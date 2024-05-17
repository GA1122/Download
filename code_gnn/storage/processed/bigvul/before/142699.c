void FrameLoader::processFragment(const KURL& url, LoadStartType loadStartType)
{
    FrameView* view = m_frame->view();
    if (!view)
        return;

    Frame* boundaryFrame = url.hasFragmentIdentifier() ? m_frame->findUnsafeParentScrollPropagationBoundary() : 0;

    if (boundaryFrame && boundaryFrame->isLocalFrame())
        toLocalFrame(boundaryFrame)->view()->setSafeToPropagateScrollToParent(false);

    bool shouldScrollToFragment = (loadStartType == NavigationWithinSameDocument && !isBackForwardLoadType(m_loadType))
        || (documentLoader() && !documentLoader()->initialScrollState().didRestoreFromHistory);

    view->processUrlFragment(url, shouldScrollToFragment ?
        FrameView::UrlFragmentScroll : FrameView::UrlFragmentDontScroll);

    if (boundaryFrame && boundaryFrame->isLocalFrame())
        toLocalFrame(boundaryFrame)->view()->setSafeToPropagateScrollToParent(true);
}
