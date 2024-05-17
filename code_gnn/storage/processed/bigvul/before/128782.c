    static v8::Local<v8::Function> createFunction(ScriptState* scriptState, ReadResult* value)
    {
        ReadResultCapturingFunction* self = new ReadResultCapturingFunction(scriptState, value);
        return self->bindToV8Function();
    }
