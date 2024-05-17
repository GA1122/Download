void V8Console::inspectCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    inspectImpl(info, false);
}
