void V8WindowShell::setSecurityToken()
{
    ASSERT(m_world->isMainWorld());

    Document* document = m_frame->document();

    SecurityOrigin* origin = document->securityOrigin();
    String token;
    if (!origin->domainWasSetInDOM()
        && !m_frame->loader()->stateMachine()->isDisplayingInitialEmptyDocument())
        token = document->securityOrigin()->toString();

    v8::HandleScope handleScope(m_isolate);
    v8::Handle<v8::Context> context = m_context.newLocal(m_isolate);
    if (token.isEmpty() || token == "null") {
        context->UseDefaultSecurityToken();
        return;
    }

    CString utf8Token = token.utf8();
    context->SetSecurityToken(v8::String::NewSymbol(utf8Token.data(), utf8Token.length()));
}
