    v8::MaybeLocal<v8::Object> firstArgAsObject()
    {
        if (m_info.Length() < 1 || !m_info[0]->IsObject())
            return v8::MaybeLocal<v8::Object>();
        return m_info[0].As<v8::Object>();
    }
