static JSObjectRef createWrappedMessage(JSGlobalContextRef context, WKStringRef data)
{
    static JSStringRef dataName = JSStringCreateWithUTF8CString("data");

    JSRetainPtr<JSStringRef> jsData = WKStringCopyJSString(data);
    JSObjectRef wrappedMessage = JSObjectMake(context, 0, 0);
    JSObjectSetProperty(context, wrappedMessage, dataName, JSValueMakeString(context, jsData.get()), kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, 0);
    return wrappedMessage;
}
