    static v8::Local<v8::Function> createFunction(ScriptState* scriptState, String* value)
    {
        StringCapturingFunction* self = new StringCapturingFunction(scriptState, value);
        return self->bindToV8Function();
    }
