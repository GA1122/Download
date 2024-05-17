    String16 firstArgToString(const String16& defaultValue)
    {
        if (m_info.Length() < 1)
            return defaultValue;
        v8::Local<v8::String> titleValue;
        if (m_info[0]->IsObject()) {
            if (!m_info[0].As<v8::Object>()->ObjectProtoToString(m_context).ToLocal(&titleValue))
                return defaultValue;
        } else {
            if (!m_info[0]->ToString(m_context).ToLocal(&titleValue))
                return defaultValue;
        }
        return toProtocolString(titleValue);
    }
