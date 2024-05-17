v8::Local<v8::Value> V8Proxy::newInstance(v8::Handle<v8::Function> constructor, int argc, v8::Handle<v8::Value> args[])
{
#if PLATFORM(CHROMIUM)
    TRACE_EVENT0("v8", "v8.newInstance");
#endif

    v8::Local<v8::Value> result;
    {
        V8RecursionScope recursionScope(frame() ? frame()->document() : 0);
        result = constructor->NewInstance(argc, args);
    }

    if (v8::V8::IsDead())
        handleFatalErrorInV8();

    return result;
}
