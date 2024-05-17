static void OverloadedMethodGMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  scheduler::CooperativeSchedulingManager::Instance()->Safepoint();

  bool is_arity_error = false;

  switch (std::min(1, info.Length())) {
    case 0:
      if (true) {
        OverloadedMethodG2Method(info);
        return;
      }
      break;
    case 1:
      if (info[0]->IsUndefined()) {
        OverloadedMethodG2Method(info);
        return;
      }
      if (IsUndefinedOrNull(info[0])) {
        OverloadedMethodG2Method(info);
        return;
      }
      if (V8TestInterfaceEmpty::HasInstance(info[0], info.GetIsolate())) {
        OverloadedMethodG2Method(info);
        return;
      }
      if (info[0]->IsNumber()) {
        OverloadedMethodG1Method(info);
        return;
      }
      if (true) {
        OverloadedMethodG1Method(info);
        return;
      }
      break;
    default:
      is_arity_error = true;
  }

  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "overloadedMethodG");
  if (is_arity_error) {
  }
  exception_state.ThrowTypeError("No function was found that matched the signature provided.");
}
