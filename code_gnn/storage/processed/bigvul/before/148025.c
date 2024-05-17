static void VoidMethodArrayBufferOrNullArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodArrayBufferOrNullArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  TestArrayBuffer* array_buffer_arg;
  array_buffer_arg = V8ArrayBuffer::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
  if (!array_buffer_arg && !IsUndefinedOrNull(info[0])) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodArrayBufferOrNullArg", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "ArrayBuffer")));
    return;
  }

  impl->voidMethodArrayBufferOrNullArg(array_buffer_arg);
}
