static void VoidMethodDefaultDoubleOrStringArgsMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodDefaultDoubleOrStringArgs");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  DoubleOrString default_long_arg;
  DoubleOrString default_string_arg;
  DoubleOrString default_null_arg;
  if (!info[0]->IsUndefined()) {
    V8DoubleOrString::ToImpl(info.GetIsolate(), info[0], default_long_arg, UnionTypeConversionMode::kNotNullable, exception_state);
    if (exception_state.HadException())
      return;
  } else {
    default_long_arg.SetDouble(10);
  }
  if (!info[1]->IsUndefined()) {
    V8DoubleOrString::ToImpl(info.GetIsolate(), info[1], default_string_arg, UnionTypeConversionMode::kNullable, exception_state);
    if (exception_state.HadException())
      return;
  } else {
    default_string_arg.SetString("foo");
  }
  if (!info[2]->IsUndefined()) {
    V8DoubleOrString::ToImpl(info.GetIsolate(), info[2], default_null_arg, UnionTypeConversionMode::kNullable, exception_state);
    if (exception_state.HadException())
      return;
  } else {
     ;
  }

  impl->voidMethodDefaultDoubleOrStringArgs(default_long_arg, default_string_arg, default_null_arg);
}
