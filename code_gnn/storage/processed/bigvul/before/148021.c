static void VoidMethodArrayBufferArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodArrayBufferArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  TestArrayBuffer* array_buffer_arg;
  array_buffer_arg = info[0]->IsArrayBuffer() ? V8ArrayBuffer::ToImpl(v8::Local<v8::ArrayBuffer>::Cast(info[0])) : 0;
  if (!array_buffer_arg) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodArrayBufferArg", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "ArrayBuffer")));
    return;
  }

  impl->voidMethodArrayBufferArg(array_buffer_arg);
}
