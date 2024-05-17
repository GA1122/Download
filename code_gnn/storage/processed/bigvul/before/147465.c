static void LimitedToOnlyAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  String cpp_value(impl->FastGetAttribute(html_names::kLimitedtoonlyattributeAttr));

  AtomicString atomic_cpp_value(cpp_value.LowerASCII());
  if (cpp_value.IsEmpty()) {
    ;
  } else if (atomic_cpp_value == keywords::kPer) {
    cpp_value = keywords::kPer;
  } else if (atomic_cpp_value == keywords::kPaal) {
    cpp_value = keywords::kPaal;
  } else if (atomic_cpp_value == keywords::kEspen) {
    cpp_value = keywords::kEspen;
  } else {
    cpp_value = g_empty_atom;
  }

  V8SetReturnValueString(info, cpp_value, info.GetIsolate());
}
