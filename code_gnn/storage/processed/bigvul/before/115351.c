void QtBuiltinBundlePage::registerNavigatorQtObject(JSGlobalContextRef context)
{
    static JSStringRef postMessageName = JSStringCreateWithUTF8CString("postMessage");
    static JSStringRef navigatorName = JSStringCreateWithUTF8CString("navigator");
    static JSStringRef qtName = JSStringCreateWithUTF8CString("qt");

    if (m_navigatorQtObject)
        JSValueUnprotect(context, m_navigatorQtObject);
    m_navigatorQtObject = JSObjectMake(context, navigatorQtObjectClass(), this);
    JSValueProtect(context, m_navigatorQtObject);

    JSObjectRef postMessage = JSObjectMakeFunctionWithCallback(context, postMessageName, qt_postMessageCallback);
    JSObjectSetProperty(context, m_navigatorQtObject, postMessageName, postMessage, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, 0);

    JSValueRef navigatorValue = JSObjectGetProperty(context, JSContextGetGlobalObject(context), navigatorName, 0);
    if (!JSValueIsObject(context, navigatorValue))
        return;
    JSObjectRef navigatorObject = JSValueToObject(context, navigatorValue, 0);
    JSObjectSetProperty(context, navigatorObject, qtName, m_navigatorQtObject, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, 0);
}
