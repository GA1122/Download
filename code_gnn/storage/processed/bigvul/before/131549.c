static void overloadedMethodGMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (((info.Length() == 0))) {
        overloadedMethodG1Method(info);
        return;
    }
    if (((info.Length() == 1) && (isUndefinedOrNull(info[0]) || info[0]->IsString() || info[0]->IsObject()))) {
        overloadedMethodG2Method(info);
        return;
    }
    throwTypeError(ExceptionMessages::failedToExecute("overloadedMethodG", "TestObjectPython", "No function was found that matched the signature provided."), info.GetIsolate());
}
