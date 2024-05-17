static void VoidMethodDocumentTypeArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodDocumentTypeArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  DocumentType* document_type_arg;
  document_type_arg = V8DocumentType::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
  if (!document_type_arg) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodDocumentTypeArg", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "DocumentType")));
    return;
  }

  impl->voidMethodDocumentTypeArg(document_type_arg);
}
