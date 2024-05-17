void TestController::setVisibilityState(WKPageVisibilityState visibilityState, bool isInitialState)
{
    WKPageSetVisibilityState(m_mainWebView->page(), visibilityState, isInitialState);
}
