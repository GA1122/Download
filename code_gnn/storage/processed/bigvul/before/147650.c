static void OverloadedMethodJ1Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8StringResource<> string_arg;
  string_arg = info[0];
  if (!string_arg.Prepare())
    return;

  impl->overloadedMethodJ(string_arg);
}
