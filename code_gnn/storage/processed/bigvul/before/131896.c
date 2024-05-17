static void voidMethodDocumentTypeArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodDocumentTypeArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(DocumentType*, documentTypeArg, V8DocumentType::toNativeWithTypeCheck(info.GetIsolate(), info[0]));
    imp->voidMethodDocumentTypeArg(documentTypeArg);
}
