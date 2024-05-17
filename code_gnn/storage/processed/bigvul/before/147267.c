static void CallWithScriptStateExecutionContextIsolateVoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  ScriptState* script_state = ScriptState::ForRelevantRealm(info);

  ExecutionContext* execution_context = ExecutionContext::ForRelevantRealm(info);
  impl->callWithScriptStateExecutionContextIsolateVoidMethod(info.GetIsolate(), script_state, execution_context);
}
