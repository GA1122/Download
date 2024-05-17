static void overloadedPerWorldBindingsMethodMethodForMainWorld(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (((info.Length() == 0))) {
        overloadedPerWorldBindingsMethod1MethodForMainWorld(info);
        return;
    }
    if (((info.Length() == 1))) {
        overloadedPerWorldBindingsMethod2MethodForMainWorld(info);
        return;
    }
    throwTypeError(ExceptionMessages::failedToExecute("overloadedPerWorldBindingsMethod", "TestObjectPython", "No function was found that matched the signature provided."), info.GetIsolate());
}
