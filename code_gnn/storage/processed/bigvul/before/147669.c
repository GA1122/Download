static void OverloadedPerWorldBindingsMethodMethodForMainWorld(const v8::FunctionCallbackInfo<v8::Value>& info) {
  scheduler::CooperativeSchedulingManager::Instance()->Safepoint();

  bool is_arity_error = false;

  switch (std::min(1, info.Length())) {
    case 0:
      if (true) {
        OverloadedPerWorldBindingsMethod1MethodForMainWorld(info);
        return;
      }
      break;
    case 1:
      if (true) {
        OverloadedPerWorldBindingsMethod2MethodForMainWorld(info);
        return;
      }
      break;
    default:
      is_arity_error = true;
  }

  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "overloadedPerWorldBindingsMethod");
  if (is_arity_error) {
  }
  exception_state.ThrowTypeError("No function was found that matched the signature provided.");
}
