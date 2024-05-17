static void overloadedMethodHMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (((info.Length() == 0))) {
        overloadedMethodH1Method(info);
        return;
    }
    if (((info.Length() == 1) && (info[0]->IsNull() || info[0]->IsFunction()))) {
        overloadedMethodH2Method(info);
        return;
    }
    throwTypeError(ExceptionMessages::failedToExecute("overloadedMethodH", "TestObjectPython", "No function was found that matched the signature provided."), info.GetIsolate());
}
