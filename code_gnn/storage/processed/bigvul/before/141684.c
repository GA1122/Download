void V8Console::logCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper(info).reportCall(ConsoleAPIType::kLog);
}
