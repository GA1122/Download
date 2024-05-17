static void DeprecateAsSameValueMeasureAsSameValueOverloadedMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  scheduler::CooperativeSchedulingManager::Instance()->Safepoint();

  Deprecation::CountDeprecation(CurrentExecutionContext(info.GetIsolate()), WebFeature::kTestFeatureA);

  bool is_arity_error = false;

  switch (std::min(1, info.Length())) {
    case 0:
      if (true) {
        ExecutionContext* execution_context_for_measurement = CurrentExecutionContext(info.GetIsolate());
        UseCounter::Count(execution_context_for_measurement, WebFeature::kTestFeatureB);
        DeprecateAsSameValueMeasureAsSameValueOverloadedMethod1Method(info);
        return;
      }
      break;
    case 1:
      if (true) {
        ExecutionContext* execution_context_for_measurement = CurrentExecutionContext(info.GetIsolate());
        UseCounter::Count(execution_context_for_measurement, WebFeature::kTestFeatureB);
        DeprecateAsSameValueMeasureAsSameValueOverloadedMethod2Method(info);
        return;
      }
      break;
    default:
      is_arity_error = true;
  }

  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "deprecateAsSameValueMeasureAsSameValueOverloadedMethod");
  if (is_arity_error) {
  }
  exception_state.ThrowTypeError("No function was found that matched the signature provided.");
}
