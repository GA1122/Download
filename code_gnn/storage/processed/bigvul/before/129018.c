    StateBase* writeDOMFileSystem(v8::Handle<v8::Value> value, StateBase* next)
    {
        DOMFileSystem* fs = V8DOMFileSystem::toNative(value.As<v8::Object>());
        if (!fs)
            return 0;
        if (!fs->clonable())
            return handleError(DataCloneError, "A FileSystem object could not be cloned.", next);
        m_writer.writeDOMFileSystem(fs->type(), fs->name(), fs->rootURL().string());
        return 0;
    }
