static void LimitedWithMissingDefaultAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  String cpp_value(impl->FastGetAttribute(html_names::kLimitedwithmissingdefaultattributeAttr));

  AtomicString atomic_cpp_value(cpp_value.LowerASCII());
  if (cpp_value.IsEmpty()) {
    cpp_value = keywords::kRsa;
  } else if (atomic_cpp_value == keywords::kRsa) {
    cpp_value = keywords::kRsa;
  } else if (atomic_cpp_value == keywords::kDsa) {
    cpp_value = keywords::kDsa;
  } else {
    cpp_value = g_empty_atom;
  }

  V8SetReturnValueString(info, cpp_value, info.GetIsolate());
}
