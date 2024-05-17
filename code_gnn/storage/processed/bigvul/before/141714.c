void V8Console::warnCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper(info).reportCall(ConsoleAPIType::kWarning);
}
