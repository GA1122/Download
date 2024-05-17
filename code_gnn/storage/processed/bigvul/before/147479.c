static void LimitedWithInvalidMissingDefaultAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  String cpp_value(impl->FastGetAttribute(html_names::kLimitedwithinvalidmissingdefaultattributeAttr));

  AtomicString atomic_cpp_value(cpp_value.LowerASCII());
  if (cpp_value.IsEmpty()) {
    cpp_value = keywords::kAuto;
  } else if (atomic_cpp_value == keywords::kLtr) {
    cpp_value = keywords::kLtr;
  } else if (atomic_cpp_value == keywords::kRtl) {
    cpp_value = keywords::kRtl;
  } else if (atomic_cpp_value == keywords::kAuto) {
    cpp_value = keywords::kAuto;
  } else {
    cpp_value = keywords::kLtr;
  }

  V8SetReturnValueString(info, cpp_value, info.GetIsolate());
}
