void V8Console::countCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper helper(info);

    String16 title = helper.firstArgToString(String16());
    String16 identifier;
    if (title.isEmpty()) {
        std::unique_ptr<V8StackTraceImpl> stackTrace = V8StackTraceImpl::capture(nullptr, 0, 1);
        if (stackTrace)
            identifier = stackTrace->topSourceURL() + ":" + String16::fromInteger(stackTrace->topLineNumber());
    } else {
        identifier = title + "@";
    }

    v8::Local<v8::Map> countMap;
    if (!helper.privateMap("V8Console#countMap").ToLocal(&countMap))
        return;
    int64_t count = helper.getIntFromMap(countMap, identifier, 0) + 1;
    helper.setIntOnMap(countMap, identifier, count);
    helper.reportCallWithArgument(ConsoleAPIType::kCount, title + ": " + String16::fromInteger(count));
}
