static void voidMethodArrayBufferViewArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodArrayBufferViewArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(ArrayBufferView*, arrayBufferViewArg, info[0]->IsArrayBufferView() ? V8ArrayBufferView::toNative(v8::Handle<v8::ArrayBufferView>::Cast(info[0])) : 0);
    imp->voidMethodArrayBufferViewArg(arrayBufferViewArg);
}
