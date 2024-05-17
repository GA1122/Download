static void VoidMethodAnyArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodAnyArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  ScriptValue any_arg;
  any_arg = ScriptValue(info.GetIsolate(), info[0]);

  impl->voidMethodAnyArg(any_arg);
}
