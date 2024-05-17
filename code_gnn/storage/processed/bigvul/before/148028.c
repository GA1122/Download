static void VoidMethodArrayOfDoubleOrDOMStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodArrayOfDoubleOrDOMStringArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  HeapVector<DoubleOrString> arg;
  arg = ToImplArguments<DoubleOrString>(info, 0, exception_state);
  if (exception_state.HadException())
    return;

  impl->voidMethodArrayOfDoubleOrDOMStringArg(arg);
}
