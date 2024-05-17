    bool checkAndSetPrivateFlagOnConsole(const char* name, bool defaultValue)
    {
        v8::Local<v8::Object> console = ensureConsole();
        v8::Local<v8::Private> key = v8::Private::ForApi(m_isolate, toV8StringInternalized(m_isolate, name));
        v8::Local<v8::Value> flagValue;
        if (!console->GetPrivate(m_context, key).ToLocal(&flagValue))
            return defaultValue;
        DCHECK(flagValue->IsUndefined() || flagValue->IsBoolean());
        if (flagValue->IsBoolean()) {
            DCHECK(flagValue.As<v8::Boolean>()->Value());
            return true;
        }
        if (!console->SetPrivate(m_context, key, v8::True(m_isolate)).FromMaybe(false))
            return defaultValue;
        return false;
    }
