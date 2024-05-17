static void UseToImpl4ArgumentsCheckingIfPossibleWithNullableArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 2)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("useToImpl4ArgumentsCheckingIfPossibleWithNullableArg", "TestObject", ExceptionMessages::NotEnoughArguments(2, info.Length())));
    return;
  }

  Node* node_1;
  Node* node_2;
  node_1 = V8Node::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
  if (!node_1) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("useToImpl4ArgumentsCheckingIfPossibleWithNullableArg", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "Node")));
    return;
  }

  node_2 = V8Node::ToImplWithTypeCheck(info.GetIsolate(), info[1]);
  if (!node_2 && !IsUndefinedOrNull(info[1])) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("useToImpl4ArgumentsCheckingIfPossibleWithNullableArg", "TestObject", ExceptionMessages::ArgumentNotOfType(1, "Node")));
    return;
  }

  impl->useToImpl4ArgumentsCheckingIfPossibleWithNullableArg(node_1, node_2);
}
