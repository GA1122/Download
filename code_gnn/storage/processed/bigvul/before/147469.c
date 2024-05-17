static void LimitedToOnlyOneAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  String cpp_value(impl->FastGetAttribute(html_names::kLimitedtoonlyoneattributeAttr));

  AtomicString atomic_cpp_value(cpp_value.LowerASCII());
  if (cpp_value.IsEmpty()) {
    ;
  } else if (atomic_cpp_value == keywords::kUnique) {
    cpp_value = keywords::kUnique;
  } else {
    cpp_value = g_empty_atom;
  }

  V8SetReturnValueString(info, cpp_value, info.GetIsolate());
}
