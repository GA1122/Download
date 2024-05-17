static void callbackFunctionReturnValueMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    v8SetReturnValue(info, imp->callbackFunctionReturnValue().v8Value());
}
