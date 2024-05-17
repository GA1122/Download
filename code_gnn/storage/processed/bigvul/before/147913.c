void V8TestObject::TestInterfaceEmptyConstructorAttributeConstructorGetterCallback(
    v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_testInterfaceEmptyConstructorAttribute_ConstructorGetterCallback");

  V8ConstructorAttributeGetter(property, info, V8TestInterfaceEmpty::GetWrapperTypeInfo());
}
