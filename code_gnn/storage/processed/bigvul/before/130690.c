static void conditionalMethod1Method(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    v8SetReturnValueString(info, imp->conditionalMethod1(), info.GetIsolate());
}
