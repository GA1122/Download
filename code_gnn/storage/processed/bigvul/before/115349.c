void QtBuiltinBundlePage::postMessageFromNavigatorQtObject(WKStringRef contents)
{
    static WKStringRef messageName = WKStringCreateWithUTF8CString("MessageFromNavigatorQtObject");
    WKTypeRef body[] = { page(), contents };
    WKRetainPtr<WKArrayRef> messageBody(AdoptWK, WKArrayCreate(body, sizeof(body) / sizeof(WKTypeRef)));
    WKBundlePostMessage(m_bundle->toRef(), messageName, messageBody.get());
}
