void TestController::didFinishLoadForFrame(WKPageRef page, WKFrameRef frame)
{
    if (m_state != Resetting)
        return;

    if (!WKFrameIsMainFrame(frame))
        return;

    WKRetainPtr<WKURLRef> wkURL(AdoptWK, WKFrameCopyURL(frame));
    if (!WKURLIsEqual(wkURL.get(), blankURL()))
        return;

    m_doneResetting = true;
    shared().notifyDone();
}
