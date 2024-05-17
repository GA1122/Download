void V8Console::infoCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper(info).reportCall(ConsoleAPIType::kInfo);
}
