static void OverloadedMethodLMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  scheduler::CooperativeSchedulingManager::Instance()->Safepoint();

  bool is_arity_error = false;

  switch (std::min(2, info.Length())) {
    case 1:
      if (info[0]->IsNumber()) {
        OverloadedMethodL1Method(info);
        return;
      }
      if (true) {
        OverloadedMethodL2Method(info);
        return;
      }
      if (true) {
        OverloadedMethodL1Method(info);
        return;
      }
      break;
    case 2:
      if (info[0]->IsNumber()) {
        OverloadedMethodL1Method(info);
        return;
      }
      if (true) {
        OverloadedMethodL2Method(info);
        return;
      }
      if (true) {
        OverloadedMethodL1Method(info);
        return;
      }
      break;
    default:
      is_arity_error = true;
  }

  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "overloadedMethodL");
  if (is_arity_error) {
    if (info.Length() < 1) {
      exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
      return;
    }
  }
  exception_state.ThrowTypeError("No function was found that matched the signature provided.");
}
