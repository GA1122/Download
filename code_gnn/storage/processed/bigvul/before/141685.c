void V8Console::markTimelineCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper(info).reportDeprecatedCall("V8Console#markTimelineDeprecated", "'console.markTimeline' is deprecated. Please use 'console.timeStamp' instead.");
    timeStampCallback(info);
}
