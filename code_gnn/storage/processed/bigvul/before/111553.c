static JSValueRef mouseMoveToCallback(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    if (argumentCount < 2)
        return JSValueMakeUndefined(context);

    int x = static_cast<int>(JSValueToNumber(context, arguments[0], exception));
    ASSERT(!exception || !*exception);
    int y = static_cast<int>(JSValueToNumber(context, arguments[1], exception));
    ASSERT(!exception || !*exception);

    lastMousePosition = IntPoint(x, y);
    BlackBerry::WebKit::WebPage* page = BlackBerry::WebKit::DumpRenderTree::currentInstance()->page();
    page->mouseEvent(BlackBerry::Platform::MouseEvent(BlackBerry::Platform::MouseEvent::ScreenLeftMouseButton, BlackBerry::Platform::MouseEvent::ScreenLeftMouseButton, lastMousePosition, IntPoint::zero(), 0, 0, 0));


    return JSValueMakeUndefined(context);
}
