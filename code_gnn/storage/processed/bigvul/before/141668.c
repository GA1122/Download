    v8::MaybeLocal<v8::Function> firstArgAsFunction()
    {
        if (m_info.Length() < 1 || !m_info[0]->IsFunction())
            return v8::MaybeLocal<v8::Function>();
        return m_info[0].As<v8::Function>();
    }
