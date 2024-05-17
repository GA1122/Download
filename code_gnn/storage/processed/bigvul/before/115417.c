void InjectedBundlePage::dump()
{
    ASSERT(InjectedBundle::shared().isTestRunning());

    InjectedBundle::shared().testRunner()->invalidateWaitToDumpWatchdogTimer();

    WKBundlePageForceRepaint(m_page);

    WKBundleFrameRef frame = WKBundlePageGetMainFrame(m_page);
    WTF::String url = toWTFString(adoptWK(WKURLCopyString(adoptWK(WKBundleFrameCopyURL(frame)).get())));
    if (url.find("dumpAsText/") != WTF::notFound)
        InjectedBundle::shared().testRunner()->dumpAsText(false);

    StringBuilder stringBuilder;

    switch (InjectedBundle::shared().testRunner()->whatToDump()) {
    case TestRunner::RenderTree: {
        WKRetainPtr<WKStringRef> text(AdoptWK, WKBundlePageCopyRenderTreeExternalRepresentation(m_page));
        stringBuilder.append(toWTFString(text));
        break;
    }
    case TestRunner::MainFrameText:
        dumpFrameText(WKBundlePageGetMainFrame(m_page), stringBuilder);
        break;
    case TestRunner::AllFramesText:
        dumpAllFramesText(stringBuilder);
        break;
    case TestRunner::Audio:
        break;
    }

    if (InjectedBundle::shared().testRunner()->shouldDumpAllFrameScrollPositions())
        dumpAllFrameScrollPositions(stringBuilder);
    else if (InjectedBundle::shared().testRunner()->shouldDumpMainFrameScrollPosition())
        dumpFrameScrollPosition(WKBundlePageGetMainFrame(m_page), stringBuilder);

    if (InjectedBundle::shared().testRunner()->shouldDumpBackForwardListsForAllWindows())
        InjectedBundle::shared().dumpBackForwardListsForAllPages(stringBuilder);

    if (InjectedBundle::shared().shouldDumpPixels() && InjectedBundle::shared().testRunner()->shouldDumpPixels()) {
        WKSnapshotOptions options = kWKSnapshotOptionsShareable | kWKSnapshotOptionsInViewCoordinates;
        if (InjectedBundle::shared().testRunner()->shouldDumpSelectionRect())
            options |= kWKSnapshotOptionsPaintSelectionRectangle;

        InjectedBundle::shared().setPixelResult(adoptWK(WKBundlePageCreateSnapshotWithOptions(m_page, WKBundleFrameGetVisibleContentBounds(WKBundlePageGetMainFrame(m_page)), options)).get());
        if (WKBundlePageIsTrackingRepaints(m_page))
            InjectedBundle::shared().setRepaintRects(adoptWK(WKBundlePageCopyTrackedRepaintRects(m_page)).get());
    }

    InjectedBundle::shared().outputText(stringBuilder.toString());
    InjectedBundle::shared().done();
}
