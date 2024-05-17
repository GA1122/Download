    v8::Handle<v8::Value> deserialize()
    {
        v8::Isolate* isolate = m_reader.scriptState()->isolate();
        if (!m_reader.readVersion(m_version) || m_version > SerializedScriptValue::wireFormatVersion)
            return v8::Null(isolate);
        m_reader.setVersion(m_version);
        v8::EscapableHandleScope scope(isolate);
        while (!m_reader.isEof()) {
            if (!doDeserialize())
                return v8::Null(isolate);
        }
        if (stackDepth() != 1 || m_openCompositeReferenceStack.size())
            return v8::Null(isolate);
        v8::Handle<v8::Value> result = scope.Escape(element(0));
        return result;
    }
