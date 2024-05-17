    void reportCallWithDefaultArgument(ConsoleAPIType type, const String16& message)
    {
        std::vector<v8::Local<v8::Value>> arguments;
        for (int i = 0; i < m_info.Length(); ++i)
            arguments.push_back(m_info[i]);
        if (!m_info.Length())
            arguments.push_back(toV8String(m_isolate, message));
        reportCall(type, arguments);
    }
