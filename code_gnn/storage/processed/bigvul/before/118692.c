bool V8WindowShell::initializeIfNeeded()
{
    if (!m_context.isEmpty())
        return true;

    TRACE_EVENT0("v8", "V8WindowShell::initializeIfNeeded");

    v8::HandleScope handleScope(m_isolate);

    V8Initializer::initializeMainThreadIfNeeded(m_isolate);

    createContext();
    if (m_context.isEmpty())
        return false;

    v8::Handle<v8::Context> context = m_context.newLocal(m_isolate);

    m_world->setIsolatedWorldField(context);

    bool isMainWorld = m_world->isMainWorld();

    v8::Context::Scope contextScope(context);

    if (m_global.isEmpty()) {
        m_global.set(m_isolate, context->Global());
        if (m_global.isEmpty()) {
            disposeContext();
            return false;
        }
    }

    if (!isMainWorld) {
        V8WindowShell* mainWindow = m_frame->script()->existingWindowShell(mainThreadNormalWorld());
        if (mainWindow && !mainWindow->context().IsEmpty())
            setInjectedScriptContextDebugId(context, m_frame->script()->contextDebugId(mainWindow->context()));
    }

    m_perContextData = V8PerContextData::create(context);
    if (!m_perContextData->init()) {
        disposeContext();
        return false;
    }
    m_perContextData->setActivityLogger(DOMWrapperWorld::activityLogger(m_world->worldId()));
    if (!installDOMWindow()) {
        disposeContext();
        return false;
    }

    if (isMainWorld) {
        updateDocument();
        setSecurityToken();
        if (m_frame->document()) {
            ContentSecurityPolicy* csp = m_frame->document()->contentSecurityPolicy();
            context->AllowCodeGenerationFromStrings(csp->allowEval(0, ContentSecurityPolicy::SuppressReport));
            context->SetErrorMessageForCodeGenerationFromStrings(v8String(csp->evalDisabledErrorMessage(), m_isolate));
        }
    } else {
        context->UseDefaultSecurityToken();

        SecurityOrigin* origin = m_world->isolatedWorldSecurityOrigin();
        if (origin && InspectorInstrumentation::hasFrontends()) {
            ScriptState* scriptState = ScriptState::forContext(v8::Local<v8::Context>::New(m_isolate, context));
            InspectorInstrumentation::didCreateIsolatedContext(m_frame, scriptState, origin);
        }
    }
    m_frame->loader()->client()->didCreateScriptContext(context, m_world->extensionGroup(), m_world->worldId());
    return true;
}
