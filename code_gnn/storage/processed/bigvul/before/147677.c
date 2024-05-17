static void PartiallyRuntimeEnabledOverloadedVoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  scheduler::CooperativeSchedulingManager::Instance()->Safepoint();

  bool is_arity_error = false;

  switch (std::min(test_object_v8_internal::PartiallyRuntimeEnabledOverloadedVoidMethodMethodMaxArg(), info.Length())) {
    case 1:
      if (RuntimeEnabledFeatures::RuntimeFeature2Enabled()) {
        if (V8TestInterface::HasInstance(info[0], info.GetIsolate())) {
          PartiallyRuntimeEnabledOverloadedVoidMethod2Method(info);
          return;
        }
      }
      if (RuntimeEnabledFeatures::RuntimeFeature1Enabled()) {
        if (true) {
          PartiallyRuntimeEnabledOverloadedVoidMethod1Method(info);
          return;
        }
      }
      break;
    case 2:
      if (true) {
        PartiallyRuntimeEnabledOverloadedVoidMethod3Method(info);
        return;
      }
      break;
    case 3:
      if (RuntimeEnabledFeatures::RuntimeFeature3Enabled()) {
        if (true) {
          PartiallyRuntimeEnabledOverloadedVoidMethod4Method(info);
          return;
        }
      }
      break;
    default:
      is_arity_error = true;
  }

  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "partiallyRuntimeEnabledOverloadedVoidMethod");
  if (is_arity_error) {
    if (info.Length() < test_object_v8_internal::PartiallyRuntimeEnabledOverloadedVoidMethodMethodLength()) {
      exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(test_object_v8_internal::PartiallyRuntimeEnabledOverloadedVoidMethodMethodLength(), info.Length()));
      return;
    }
  }
  exception_state.ThrowTypeError("No function was found that matched the signature provided.");
}
