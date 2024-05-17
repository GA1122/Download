void WebPage::executeJavaScriptFunction(const std::vector<BlackBerry::Platform::String> &function, const std::vector<JavaScriptVariant> &args, JavaScriptVariant& returnValue)
{
    if (!d->m_mainFrame) {
        returnValue.setType(JavaScriptVariant::Exception);
        return;
    }

    JSC::Bindings::RootObject* root = d->m_mainFrame->script()->bindingRootObject();
    if (!root) {
        returnValue.setType(JavaScriptVariant::Exception);
        return;
    }

    JSC::ExecState* exec = root->globalObject()->globalExec();
    JSGlobalContextRef ctx = toGlobalRef(exec);

    JSC::JSLockHolder lock(exec);
    WTF::Vector<JSValueRef> argListRef(args.size());
    for (unsigned i = 0; i < args.size(); ++i)
        argListRef[i] = BlackBerryJavaScriptVariantToJSValueRef(ctx, args[i]);

    JSValueRef windowObjectValue = windowObject();
    JSObjectRef obj = JSValueToObject(ctx, windowObjectValue, 0);
    JSObjectRef thisObject = obj;
    for (unsigned i = 0; i < function.size(); ++i) {
        JSStringRef str = JSStringCreateWithUTF8CString(function[i].c_str());
        thisObject = obj;
        obj = JSValueToObject(ctx, JSObjectGetProperty(ctx, obj, str, 0), 0);
        JSStringRelease(str);
        if (!obj)
            break;
    }

    JSObjectRef functionObject = obj;
    JSValueRef result = 0;
    if (functionObject && thisObject)
        result = JSObjectCallAsFunction(ctx, functionObject, thisObject, args.size(), argListRef.data(), 0);

    if (!result) {
        returnValue.setType(JavaScriptVariant::Exception);
        return;
    }

    returnValue = JSValueRefToBlackBerryJavaScriptVariant(ctx, result);
}
