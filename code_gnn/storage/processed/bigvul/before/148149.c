static void VoidMethodSerializedScriptValueArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodSerializedScriptValueArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  scoped_refptr<SerializedScriptValue> serialized_script_value_arg;
  serialized_script_value_arg = NativeValueTraits<SerializedScriptValue>::NativeValue(info.GetIsolate(), info[0], SerializedScriptValue::SerializeOptions(SerializedScriptValue::kNotForStorage), exception_state);
  if (exception_state.HadException())
    return;

  impl->voidMethodSerializedScriptValueArg(std::move(serialized_script_value_arg));
}
