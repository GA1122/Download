    bool readDOMFileSystem(v8::Handle<v8::Value>* value)
    {
        uint32_t type;
        String name;
        String url;
        if (!doReadUint32(&type))
            return false;
        if (!readWebCoreString(&name))
            return false;
        if (!readWebCoreString(&url))
            return false;
        DOMFileSystem* fs = DOMFileSystem::create(m_scriptState->executionContext(), name, static_cast<blink::FileSystemType>(type), KURL(ParsedURLString, url));
        *value = toV8(fs, m_scriptState->context()->Global(), isolate());
        return true;
    }
