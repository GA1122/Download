static void VoidMethodDefaultStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8StringResource<> default_string_arg;
  if (!info[0]->IsUndefined()) {
    default_string_arg = info[0];
    if (!default_string_arg.Prepare())
      return;
  } else {
    default_string_arg = "foo";
  }

  impl->voidMethodDefaultStringArg(default_string_arg);
}
