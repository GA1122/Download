static void checkSecurityForNodeReadonlyDocumentAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    ExceptionState exceptionState(ExceptionState::GetterContext, "checkSecurityForNodeReadonlyDocumentAttribute", "TestObjectPython", info.Holder(), info.GetIsolate());
    if (!BindingSecurity::shouldAllowAccessToNode(info.GetIsolate(), imp->checkSecurityForNodeReadonlyDocumentAttribute(), exceptionState)) {
        v8SetReturnValueNull(info);
        exceptionState.throwIfNeeded();
        return;
    }
    v8SetReturnValueFast(info, WTF::getPtr(imp->checkSecurityForNodeReadonlyDocumentAttribute()), imp);
}
