    void reportDeprecatedCall(const char* id, const String16& message)
    {
        if (checkAndSetPrivateFlagOnConsole(id, false))
            return;
        std::vector<v8::Local<v8::Value>> arguments(1, toV8String(m_isolate, message));
        reportCall(ConsoleAPIType::kWarning, arguments);
    }
