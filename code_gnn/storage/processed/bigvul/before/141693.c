    void reportCall(ConsoleAPIType type)
    {
        if (!m_info.Length())
            return;
        std::vector<v8::Local<v8::Value>> arguments;
        for (int i = 0; i < m_info.Length(); ++i)
            arguments.push_back(m_info[i]);
        reportCall(type, arguments);
    }
