static void TestEnumAttributeAttributeSetter(
    v8::Local<v8::Value> v8_value, const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  ALLOW_UNUSED_LOCAL(isolate);

  v8::Local<v8::Object> holder = info.Holder();
  ALLOW_UNUSED_LOCAL(holder);

  TestObject* impl = V8TestObject::ToImpl(holder);

  ExceptionState exception_state(isolate, ExceptionState::kSetterContext, "TestObject", "testEnumAttribute");

  V8StringResource<> cpp_value = v8_value;
  if (!cpp_value.Prepare())
    return;

  DummyExceptionStateForTesting dummy_exception_state;
  {
    const char* const kValidValues[] = {
      "",
      "EnumValue1",
      "EnumValue2",
      "EnumValue3",
  };
    if (!IsValidEnum(cpp_value, kValidValues, base::size(kValidValues),
                     "TestEnum", dummy_exception_state)) {
      ExecutionContext::ForCurrentRealm(info)->AddConsoleMessage(
          ConsoleMessage::Create(mojom::ConsoleMessageSource::kJavaScript,
                                 mojom::ConsoleMessageLevel::kWarning,
                                 dummy_exception_state.Message()));
      return;
    }
  }

  impl->setTestEnumAttribute(cpp_value);
}
