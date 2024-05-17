v8::Local<v8::Value> V8Proxy::evaluate(const ScriptSourceCode& source, Node* node)
{
    ASSERT(v8::Context::InContext());

    V8GCController::checkMemoryUsage();

    InspectorInstrumentationCookie cookie = InspectorInstrumentation::willEvaluateScript(m_frame, source.url().isNull() ? String() : source.url().string(), source.startLine());

    v8::Local<v8::Value> result;
    {
        v8::TryCatch tryCatch;
        tryCatch.SetVerbose(true);

        v8::Local<v8::String> code = v8ExternalString(source.source());
#if PLATFORM(CHROMIUM)
        TRACE_EVENT_BEGIN0("v8", "v8.compile");
#endif
        OwnPtr<v8::ScriptData> scriptData = precompileScript(code, source.cachedScript());

        v8::Handle<v8::Script> script = compileScript(code, source.url(), source.startPosition(), scriptData.get());
#if PLATFORM(CHROMIUM)
        TRACE_EVENT_END0("v8", "v8.compile");
        TRACE_EVENT0("v8", "v8.run");
#endif
        result = runScript(script);
    }

    InspectorInstrumentation::didEvaluateScript(cookie);

    return result;
}
