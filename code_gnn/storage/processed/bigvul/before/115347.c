void QtBuiltinBundlePage::didReceiveMessageToNavigatorQtObject(WKStringRef contents)
{
    static JSStringRef onmessageName = JSStringCreateWithUTF8CString("onmessage");

    if (!m_navigatorQtObject)
        return;

    WKBundleFrameRef frame = WKBundlePageGetMainFrame(m_page);
    JSGlobalContextRef context = WKBundleFrameGetJavaScriptContext(frame);

    JSValueRef onmessageValue = JSObjectGetProperty(context, m_navigatorQtObject, onmessageName, 0);
    if (!JSValueIsObject(context, onmessageValue))
        return;

    JSObjectRef onmessageFunction = JSValueToObject(context, onmessageValue, 0);
    if (!JSObjectIsFunction(context, onmessageFunction))
        return;

    JSObjectRef wrappedMessage = createWrappedMessage(context, contents);
    JSObjectCallAsFunction(context, onmessageFunction, 0, 1, &wrappedMessage, 0);
}
