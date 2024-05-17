static JSValueRef clearTouchPointsCallback(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    touches.clear();
    touchActive = false;
    return JSValueMakeUndefined(context);
}
