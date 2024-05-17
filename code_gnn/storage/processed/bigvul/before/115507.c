void TestController::didCommitLoadForFrame(WKPageRef page, WKFrameRef frame)
{
    if (!WKFrameIsMainFrame(frame))
        return;

    mainWebView()->focus();
}
