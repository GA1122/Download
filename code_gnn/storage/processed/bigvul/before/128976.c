    bool readFileList(v8::Handle<v8::Value>* value, bool isIndexed)
    {
        if (m_version < 3)
            return false;
        uint32_t length;
        if (!doReadUint32(&length))
            return false;
        RefPtrWillBeRawPtr<FileList> fileList = FileList::create();
        for (unsigned i = 0; i < length; ++i) {
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
            fileList->append(file.release());
        }
        *value = toV8(fileList.release(), m_scriptState->context()->Global(), isolate());
        return true;
    }
