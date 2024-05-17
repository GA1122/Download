static void VoidMethodDefaultUndefinedStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8StringResource<> default_undefined_string_arg;
  default_undefined_string_arg = info[0];
  if (!default_undefined_string_arg.Prepare())
    return;

  impl->voidMethodDefaultUndefinedStringArg(default_undefined_string_arg);
}
