static void conditionalCondition1AndCondition2VoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    imp->conditionalCondition1AndCondition2VoidMethod();
}
