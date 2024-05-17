static JSValueRef touchMoveCallback(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    sendTouchEvent(BlackBerry::Platform::TouchEvent::TouchMove);
    return JSValueMakeUndefined(context);
}
