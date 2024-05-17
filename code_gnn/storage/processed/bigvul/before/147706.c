static void PromiseMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "promiseMethod");
  ExceptionToRejectPromiseScope reject_promise_scope(info, exception_state);

  if (!V8TestObject::HasInstance(info.Holder(), info.GetIsolate())) {
    exception_state.ThrowTypeError("Illegal invocation");
    return;
  }
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 3)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(3, info.Length()));
    return;
  }

  int32_t arg_1;
  Dictionary arg_2;
  V8StringResource<> arg_3;
  Vector<String> variadic;
  arg_1 = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  if (!info[1]->IsNullOrUndefined() && !info[1]->IsObject()) {
    exception_state.ThrowTypeError("parameter 2 ('arg2') is not an object.");
    return;
  }
  arg_2 = NativeValueTraits<Dictionary>::NativeValue(info.GetIsolate(), info[1], exception_state);
  if (exception_state.HadException())
    return;

  arg_3 = info[2];
  if (!arg_3.Prepare(exception_state))
    return;

  variadic = ToImplArguments<IDLString>(info, 3, exception_state);
  if (exception_state.HadException())
    return;

  V8SetReturnValue(info, impl->promiseMethod(arg_1, arg_2, arg_3, variadic).V8Value());
}
