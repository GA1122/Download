    bool readArrayBuffer(v8::Handle<v8::Value>* value)
    {
        RefPtr<ArrayBuffer> arrayBuffer = doReadArrayBuffer();
        if (!arrayBuffer)
            return false;
        *value = toV8(arrayBuffer.release(), m_scriptState->context()->Global(), isolate());
        return true;
    }
