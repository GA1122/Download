    InspectedContext* ensureInspectedContext()
    {
        if (m_inspectedContext)
            return m_inspectedContext;
        v8::Local<v8::Object> console = ensureConsole();

        v8::Local<v8::Private> key = inspectedContextPrivateKey(m_isolate);
        v8::Local<v8::Value> inspectedContextValue;
        if (!console->GetPrivate(m_context, key).ToLocal(&inspectedContextValue))
            return nullptr;
        DCHECK(inspectedContextValue->IsExternal());
        m_inspectedContext = static_cast<InspectedContext*>(inspectedContextValue.As<v8::External>()->Value());
        return m_inspectedContext;
    }
