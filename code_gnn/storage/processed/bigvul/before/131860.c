static void voidMethodArrayBufferArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodArrayBufferArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(ArrayBuffer*, arrayBufferArg, info[0]->IsArrayBuffer() ? V8ArrayBuffer::toNative(v8::Handle<v8::ArrayBuffer>::Cast(info[0])) : 0);
    imp->voidMethodArrayBufferArg(arrayBufferArg);
}
