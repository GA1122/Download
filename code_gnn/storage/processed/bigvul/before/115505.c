void TestController::decidePolicyForResponse(WKFrameRef frame, WKURLResponseRef response, WKFramePolicyListenerRef listener)
{
    WKRetainPtr<WKStringRef> wkMIMEType(AdoptWK, WKURLResponseCopyMIMEType(response));
    if (WKFrameCanShowMIMEType(frame, wkMIMEType.get())) {
        WKFramePolicyListenerUse(listener);
        return;
    }

    WKFramePolicyListenerIgnore(listener);
}
