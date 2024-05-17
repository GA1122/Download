static JSValueRef scalePageByCallback(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    if (argumentCount < 3)
        return JSValueMakeUndefined(context);

    float scaleFactor = JSValueToNumber(context, arguments[0], exception);
    float x = JSValueToNumber(context, arguments[1], exception);
    float y = JSValueToNumber(context, arguments[2], exception);

    BlackBerry::WebKit::WebPage* page = BlackBerry::WebKit::DumpRenderTree::currentInstance()->page();
    if (!page)
        return JSValueMakeUndefined(context);

    DumpRenderTreeSupport::scalePageBy(page, scaleFactor, x, y);

    return JSValueMakeUndefined(context);
}
