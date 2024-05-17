static void voidMethodFloat32ArrayArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodFloat32ArrayArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Float32Array*, float32ArrayArg, info[0]->IsFloat32Array() ? V8Float32Array::toNative(v8::Handle<v8::Float32Array>::Cast(info[0])) : 0);
    imp->voidMethodFloat32ArrayArg(float32ArrayArg);
}
