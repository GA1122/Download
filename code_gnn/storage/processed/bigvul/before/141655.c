void V8Console::copyCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    inspectImpl(info, true);
}
