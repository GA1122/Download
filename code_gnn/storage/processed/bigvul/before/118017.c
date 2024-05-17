v8::Local<v8::Value> V8Proxy::runScript(v8::Handle<v8::Script> script)
{
    if (script.IsEmpty())
        return notHandledByInterceptor();

    V8GCController::checkMemoryUsage();
    if (V8RecursionScope::recursionLevel() >= kMaxRecursionDepth)
        return handleMaxRecursionDepthExceeded();

    if (handleOutOfMemory())
        ASSERT(script.IsEmpty());

    RefPtr<Frame> protect(frame());

    v8::Local<v8::Value> result;
    v8::TryCatch tryCatch;
    tryCatch.SetVerbose(true);
    {
        V8RecursionScope recursionScope(frame()->document());
        result = script->Run();
    }

    if (handleOutOfMemory())
        ASSERT(result.IsEmpty());

    if (tryCatch.HasCaught()) {
        ASSERT(result.IsEmpty());
        return notHandledByInterceptor();
    }

    if (result.IsEmpty())
        return notHandledByInterceptor();

    if (v8::V8::IsDead())
        handleFatalErrorInV8();

    return result;
}
