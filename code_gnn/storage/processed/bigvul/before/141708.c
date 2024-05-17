void V8Console::timelineCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper(info).reportDeprecatedCall("V8Console#timeline", "'console.timeline' is deprecated. Please use 'console.time' instead.");
    timeFunction(info, true);
}
