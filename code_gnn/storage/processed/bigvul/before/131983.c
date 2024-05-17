static void voidMethodUint8ArrayArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodUint8ArrayArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Uint8Array*, uint8ArrayArg, info[0]->IsUint8Array() ? V8Uint8Array::toNative(v8::Handle<v8::Uint8Array>::Cast(info[0])) : 0);
    imp->voidMethodUint8ArrayArg(uint8ArrayArg);
}
