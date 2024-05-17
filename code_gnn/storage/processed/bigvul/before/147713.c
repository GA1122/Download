static void PromiseOverloadMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  scheduler::CooperativeSchedulingManager::Instance()->Safepoint();

  bool is_arity_error = false;

  switch (std::min(2, info.Length())) {
    case 0:
      if (true) {
        PromiseOverloadMethod1Method(info);
        return;
      }
      break;
    case 2:
      if (V8Window::HasInstance(info[0], info.GetIsolate())) {
        PromiseOverloadMethod2Method(info);
        return;
      }
      if (V8Document::HasInstance(info[0], info.GetIsolate())) {
        PromiseOverloadMethod3Method(info);
        return;
      }
      break;
    default:
      is_arity_error = true;
  }

  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "promiseOverloadMethod");
  ExceptionToRejectPromiseScope reject_promise_scope(info, exception_state);
  if (is_arity_error) {
    if (info.Length() >= 0) {
      exception_state.ThrowTypeError(ExceptionMessages::InvalidArity("[0, 2]", info.Length()));
      return;
    }
  }
  exception_state.ThrowTypeError("No function was found that matched the signature provided.");
}
