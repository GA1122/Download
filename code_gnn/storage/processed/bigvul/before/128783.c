    v8::Local<v8::Function> createResultCaptor(ReadResult* value)
    {
        return ReadResultCapturingFunction::createFunction(getScriptState(), value);
    }
