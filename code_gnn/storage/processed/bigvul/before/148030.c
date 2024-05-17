static void VoidMethodAttrArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodAttrArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  Attr* attr_arg;
  attr_arg = V8Attr::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
  if (!attr_arg) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodAttrArg", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "Attr")));
    return;
  }

  impl->voidMethodAttrArg(attr_arg);
}
