static void CheckSecurityForNodeVoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "checkSecurityForNodeVoidMethod");
  if (!BindingSecurity::ShouldAllowAccessTo(
          CurrentDOMWindow(info.GetIsolate()), impl->checkSecurityForNodeVoidMethod(),
          BindingSecurity::ErrorReportOption::kDoNotReport)) {
    UseCounter::Count(CurrentExecutionContext(info.GetIsolate()),
                      WebFeature::kCrossOriginTestObjectCheckSecurityForNodeVoidMethod);
    V8SetReturnValueNull(info);
    return;
  }

  impl->checkSecurityForNodeVoidMethod();
}
