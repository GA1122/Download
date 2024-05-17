void ScriptController::disableEval()
{
    if (!m_proxy->windowShell()->initContextIfNeeded())
        return;

    v8::HandleScope handleScope;
    v8::Handle<v8::Context> v8Context = V8Proxy::mainWorldContext(m_frame);
    if (v8Context.IsEmpty())
        return;

    v8Context->AllowCodeGenerationFromStrings(false);
}
