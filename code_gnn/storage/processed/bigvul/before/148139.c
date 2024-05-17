static void VoidMethodPromiseArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodPromiseArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  ScriptPromise promise_arg;
  promise_arg = ScriptPromise::Cast(ScriptState::Current(info.GetIsolate()), info[0]);
  if (!promise_arg.IsUndefinedOrNull() && !promise_arg.IsObject()) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodPromiseArg", "TestObject", "parameter 1 ('promiseArg') is not an object."));
    return;
  }

  impl->voidMethodPromiseArg(promise_arg);
}
