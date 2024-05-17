static void voidMethodTreatNullAsNullStringStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodTreatNullAsNullStringStringArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<WithNullCheck>, treatNullAsNullStringStringArg, info[0]);
    imp->voidMethodTreatNullAsNullStringStringArg(treatNullAsNullStringStringArg);
}
