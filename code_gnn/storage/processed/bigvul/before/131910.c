static void voidMethodInt32ArrayArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodInt32ArrayArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Int32Array*, int32ArrayArg, info[0]->IsInt32Array() ? V8Int32Array::toNative(v8::Handle<v8::Int32Array>::Cast(info[0])) : 0);
    imp->voidMethodInt32ArrayArg(int32ArrayArg);
}
