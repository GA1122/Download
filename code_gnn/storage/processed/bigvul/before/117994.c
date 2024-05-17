v8::Local<v8::Value> V8Proxy::instrumentedCallFunction(Frame* frame, v8::Handle<v8::Function> function, v8::Handle<v8::Object> receiver, int argc, v8::Handle<v8::Value> args[])
{
    V8GCController::checkMemoryUsage();

    if (V8RecursionScope::recursionLevel() >= kMaxRecursionDepth)
        return handleMaxRecursionDepthExceeded();

    ScriptExecutionContext* context = frame ? frame->document() : 0;

    InspectorInstrumentationCookie cookie;
    if (InspectorInstrumentation::hasFrontends() && context) {
        String resourceName;
        int lineNumber;
        resourceInfo(function, resourceName, lineNumber);
        cookie = InspectorInstrumentation::willCallFunction(context, resourceName, lineNumber);
    }

    v8::Local<v8::Value> result;
    {
#if PLATFORM(CHROMIUM)
        TRACE_EVENT1("v8", "v8.callFunction", "callsite", resourceString(function).utf8());
#endif
        V8RecursionScope recursionScope(context);
        result = function->Call(receiver, argc, args);
    }

    InspectorInstrumentation::didCallFunction(cookie);

    if (v8::V8::IsDead())
        handleFatalErrorInV8();

    return result;
}
