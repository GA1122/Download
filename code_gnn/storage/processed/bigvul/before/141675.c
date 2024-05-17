void V8Console::groupCollapsedCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper(info).reportCallWithDefaultArgument(ConsoleAPIType::kStartGroupCollapsed, String16("console.groupCollapsed"));
}
