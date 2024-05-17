void V8Console::timeEndCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    timeEndFunction(info, false);
}
