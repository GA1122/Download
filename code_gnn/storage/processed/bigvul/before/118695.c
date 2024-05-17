static void setInjectedScriptContextDebugId(v8::Handle<v8::Context> targetContext, int debugId)
{
    V8PerContextDebugData::setContextDebugData(targetContext, "injected", debugId);
}
