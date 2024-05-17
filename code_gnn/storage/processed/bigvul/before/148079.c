static void VoidMethodDocumentArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodDocumentArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  Document* document_arg;
  document_arg = V8Document::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
  if (!document_arg) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodDocumentArg", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "Document")));
    return;
  }

  impl->voidMethodDocumentArg(document_arg);
}
