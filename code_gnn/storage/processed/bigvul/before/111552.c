static JSValueRef mouseDownCallback(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    BlackBerry::WebKit::WebPage* page = BlackBerry::WebKit::DumpRenderTree::currentInstance()->page();
    page->mouseEvent(BlackBerry::Platform::MouseEvent(BlackBerry::Platform::MouseEvent::ScreenLeftMouseButton, 0, lastMousePosition, IntPoint::zero(), 0, 0, 0));
    return JSValueMakeUndefined(context);
}
