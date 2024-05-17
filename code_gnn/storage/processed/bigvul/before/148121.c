static void VoidMethodNodeArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodNodeArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  Node* node_arg;
  node_arg = V8Node::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
  if (!node_arg) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodNodeArg", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "Node")));
    return;
  }

  impl->voidMethodNodeArg(node_arg);
}
