    bool readFile(v8::Handle<v8::Value>* value, bool isIndexed)
    {
        RefPtrWillBeRawPtr<File> file;
        if (isIndexed) {
            if (m_version < 6)
                return false;
            file = readFileIndexHelper();
        } else {
            file = readFileHelper();
        }
        if (!file)
            return false;
        *value = toV8(file.release(), m_scriptState->context()->Global(), isolate());
        return true;
    }
