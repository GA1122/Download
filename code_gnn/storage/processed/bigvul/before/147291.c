static void CorsSettingAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  String cpp_value(impl->FastGetAttribute(html_names::kCorssettingattributeAttr));

  AtomicString atomic_cpp_value(cpp_value.LowerASCII());
  if (cpp_value.IsNull()) {
    ;
  } else if (cpp_value.IsEmpty()) {
    cpp_value = keywords::kAnonymous;
  } else if (atomic_cpp_value == keywords::kAnonymous) {
    cpp_value = keywords::kAnonymous;
  } else if (atomic_cpp_value == keywords::kUseCredentials) {
    cpp_value = keywords::kUseCredentials;
  } else {
    cpp_value = keywords::kAnonymous;
  }

  V8SetReturnValueString(info, cpp_value, info.GetIsolate());
}
