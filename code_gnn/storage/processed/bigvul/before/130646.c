static void attrWithJSGetterAndSetterAttributeSetter(v8::Local<v8::Value> jsValue, const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, cppValue, jsValue);
    imp->setAttrWithJSGetterAndSetter(cppValue);
}
