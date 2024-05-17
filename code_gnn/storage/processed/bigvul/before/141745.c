int V8Debugger::markContext(const V8ContextInfo& info)
{
    DCHECK(info.context->GetIsolate() == m_isolate);
    int contextId = ++m_lastContextId;
    String16 debugData = String16::fromInteger(info.contextGroupId) + "," + String16::fromInteger(contextId) + "," + info.auxData;
    v8::Context::Scope contextScope(info.context);
    info.context->SetEmbedderData(static_cast<int>(v8::Context::kDebugIdIndex), toV8String(m_isolate, debugData));
    return contextId;
}
