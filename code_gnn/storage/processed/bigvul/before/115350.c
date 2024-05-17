static JSValueRef qt_postMessageCallback(JSContextRef context, JSObjectRef, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef*)
{

    if (argumentCount < 1 || !JSValueIsString(context, arguments[0]))
        return JSValueMakeUndefined(context);

    QtBuiltinBundlePage* bundlePage = reinterpret_cast<QtBuiltinBundlePage*>(JSObjectGetPrivate(thisObject));
    ASSERT(bundlePage);

    if (!bundlePage->navigatorQtObjectEnabled())
        return JSValueMakeUndefined(context);

    JSRetainPtr<JSStringRef> jsContents = JSValueToStringCopy(context, arguments[0], 0);
    WKRetainPtr<WKStringRef> contents(AdoptWK, WKStringCreateWithJSString(jsContents.get()));
    bundlePage->postMessageFromNavigatorQtObject(contents.get());
    return JSValueMakeUndefined(context);
}
