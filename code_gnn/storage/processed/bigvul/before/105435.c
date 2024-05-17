WebKitHitTestResult* webkit_web_view_get_hit_test_result(WebKitWebView* webView, GdkEventButton* event)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), NULL);
    g_return_val_if_fail(event, NULL);

    PlatformMouseEvent mouseEvent = PlatformMouseEvent(event);
    Frame* frame = core(webView)->focusController()->focusedOrMainFrame();
    HitTestRequest request(HitTestRequest::Active);
    IntPoint documentPoint = documentPointForWindowPoint(frame, mouseEvent.pos());
    MouseEventWithHitTestResults mev = frame->document()->prepareMouseEvent(request, documentPoint, mouseEvent);

    return kit(mev.hitTestResult());
}
