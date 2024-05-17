static void voidMethodStringArgVariadicStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodStringArgVariadicStringArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, stringArg, info[0]);
    V8TRYCATCH_VOID(Vector<String>, variadicStringArgs, toNativeArguments<String>(info, 1));
    imp->voidMethodStringArgVariadicStringArg(stringArg, variadicStringArgs);
}
