static void DeprecateAsMeasureAsSameValueOverloadedMethod1Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  impl->deprecateAsMeasureAsSameValueOverloadedMethod();
}