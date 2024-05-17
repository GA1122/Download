void V8Console::clearCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper(info).reportCallWithDefaultArgument(ConsoleAPIType::kClear, String16("console.clear"));
}
