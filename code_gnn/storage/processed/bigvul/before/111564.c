static JSValueRef touchStartCallback(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    if (!touchActive) {
        sendTouchEvent(BlackBerry::Platform::TouchEvent::TouchStart);
        touchActive = true;
    } else
        sendTouchEvent(BlackBerry::Platform::TouchEvent::TouchMove);
    return JSValueMakeUndefined(context);
}
