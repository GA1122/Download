void V8Console::groupCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper(info).reportCallWithDefaultArgument(ConsoleAPIType::kStartGroup, String16("console.group"));
}
