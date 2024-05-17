bool V8Proxy::setContextDebugId(int debugId)
{
    ASSERT(debugId > 0);
    v8::HandleScope scope;
    v8::Handle<v8::Context> context = windowShell()->context();
    if (context.IsEmpty())
        return false;
    if (!context->GetData()->IsUndefined())
        return false;

    v8::Context::Scope contextScope(context);

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "page,%d", debugId);
    context->SetData(v8::String::New(buffer));

    return true;
}
