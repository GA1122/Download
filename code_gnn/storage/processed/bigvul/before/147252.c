static void CallWithExecutionContextAndScriptStateAndIsolateAnyAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  ExecutionContext* execution_context = ExecutionContext::ForRelevantRealm(info);

  ScriptState* script_state = ScriptState::ForRelevantRealm(info);

  V8SetReturnValue(info, impl->callWithExecutionContextAndScriptStateAndIsolateAnyAttribute(info.GetIsolate(), script_state, execution_context).V8Value());
}
