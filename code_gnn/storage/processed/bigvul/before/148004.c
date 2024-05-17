static void UseToImpl4ArgumentsCheckingIfPossibleWithOptionalArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("useToImpl4ArgumentsCheckingIfPossibleWithOptionalArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  Node* node_1;
  Node* node_2;
  int num_args_passed = info.Length();
  while (num_args_passed > 0) {
    if (!info[num_args_passed - 1]->IsUndefined())
      break;
    --num_args_passed;
  }
  node_1 = V8Node::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
  if (!node_1) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("useToImpl4ArgumentsCheckingIfPossibleWithOptionalArg", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "Node")));
    return;
  }

  if (UNLIKELY(num_args_passed <= 1)) {
    impl->useToImpl4ArgumentsCheckingIfPossibleWithOptionalArg(node_1);
    return;
  }
  node_2 = V8Node::ToImplWithTypeCheck(info.GetIsolate(), info[1]);
  if (!node_2) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("useToImpl4ArgumentsCheckingIfPossibleWithOptionalArg", "TestObject", ExceptionMessages::ArgumentNotOfType(1, "Node")));
    return;
  }

  impl->useToImpl4ArgumentsCheckingIfPossibleWithOptionalArg(node_1, node_2);
}
