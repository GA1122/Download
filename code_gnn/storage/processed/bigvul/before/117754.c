static void supplementalStr2AttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestInterface.supplementalStr2._set");
    TestInterface* imp = V8TestInterface::toNative(info.Holder());
    STRING_TO_V8PARAMETER_EXCEPTION_BLOCK_VOID(V8Parameter<>, v, value);
    TestSupplemental::setSupplementalStr2(imp, v);
    return;
}
