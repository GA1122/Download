static void overloadedPerWorldBindingsMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (((info.Length() == 0))) {
        overloadedPerWorldBindingsMethod1Method(info);
        return;
    }
    if (((info.Length() == 1))) {
        overloadedPerWorldBindingsMethod2Method(info);
        return;
    }
    throwTypeError(ExceptionMessages::failedToExecute("overloadedPerWorldBindingsMethod", "TestObjectPython", "No function was found that matched the signature provided."), info.GetIsolate());
}
