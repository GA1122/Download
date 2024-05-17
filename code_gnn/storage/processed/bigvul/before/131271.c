static void checkSecurityForNodeVoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    if (!BindingSecurity::shouldAllowAccessToNode(info.GetIsolate(), imp->checkSecurityForNodeVoidMethod(exceptionState), exceptionState)) {
        v8SetReturnValueNull(info);
        exceptionState.throwIfNeeded();
        return;
    }
    imp->checkSecurityForNodeVoidMethod();
}
