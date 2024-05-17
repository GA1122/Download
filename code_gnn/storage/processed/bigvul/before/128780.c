    v8::Local<v8::Function> createCaptor(String* value)
    {
        return StringCapturingFunction::createFunction(getScriptState(), value);
    }
