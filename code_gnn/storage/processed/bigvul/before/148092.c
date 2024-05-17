static void VoidMethodElementArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodElementArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  Element* element_arg;
  element_arg = V8Element::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
  if (!element_arg) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodElementArg", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "Element")));
    return;
  }

  impl->voidMethodElementArg(element_arg);
}
