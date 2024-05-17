void V8Console::timelineEndCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper(info).reportDeprecatedCall("V8Console#timelineEnd", "'console.timelineEnd' is deprecated. Please use 'console.timeEnd' instead.");
    timeEndFunction(info, true);
}
