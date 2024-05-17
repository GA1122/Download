static void VoidMethodOptionalStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8StringResource<> optional_string_arg;
  int num_args_passed = info.Length();
  while (num_args_passed > 0) {
    if (!info[num_args_passed - 1]->IsUndefined())
      break;
    --num_args_passed;
  }
  if (UNLIKELY(num_args_passed <= 0)) {
    impl->voidMethodOptionalStringArg();
    return;
  }
  optional_string_arg = info[0];
  if (!optional_string_arg.Prepare())
    return;

  impl->voidMethodOptionalStringArg(optional_string_arg);
}
