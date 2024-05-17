static void LimitedWithEmptyMissingInvalidAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  String cpp_value(impl->FastGetAttribute(html_names::kLimitedwithemptymissinginvalidattributeAttr));

  AtomicString atomic_cpp_value(cpp_value.LowerASCII());
  if (cpp_value.IsNull()) {
    cpp_value = keywords::kMissing;
  } else if (cpp_value.IsEmpty()) {
    cpp_value = keywords::kEmpty;
  } else if (atomic_cpp_value == keywords::kEmpty) {
    cpp_value = keywords::kEmpty;
  } else if (atomic_cpp_value == keywords::kMissing) {
    cpp_value = keywords::kMissing;
  } else if (atomic_cpp_value == keywords::kInvalid) {
    cpp_value = keywords::kInvalid;
  } else if (atomic_cpp_value == keywords::kANormal) {
    cpp_value = keywords::kANormal;
  } else {
    cpp_value = keywords::kInvalid;
  }

  V8SetReturnValueString(info, cpp_value, info.GetIsolate());
}
