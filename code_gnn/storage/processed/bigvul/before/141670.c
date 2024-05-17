    bool firstArgToBoolean(bool defaultValue)
    {
        if (m_info.Length() < 1)
            return defaultValue;
        if (m_info[0]->IsBoolean())
            return m_info[0].As<v8::Boolean>()->Value();
        return m_info[0]->BooleanValue(m_context).FromMaybe(defaultValue);
    }
