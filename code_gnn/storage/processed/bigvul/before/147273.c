static void CallWithThisValueMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  impl->callWithThisValue(ScriptValue(info.GetIsolate(), info.Holder()));
}
