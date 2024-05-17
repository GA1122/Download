static void CallWithExecutionContextVoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  ExecutionContext* execution_context = ExecutionContext::ForRelevantRealm(info);
  impl->callWithExecutionContextVoidMethod(execution_context);
}
