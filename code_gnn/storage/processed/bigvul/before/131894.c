static void voidMethodDocumentArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodDocumentArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Document*, documentArg, V8Document::toNativeWithTypeCheck(info.GetIsolate(), info[0]));
    imp->voidMethodDocumentArg(documentArg);
}
