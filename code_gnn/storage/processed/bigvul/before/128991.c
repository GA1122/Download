    Status serialize(v8::Handle<v8::Value> value)
    {
        v8::HandleScope scope(isolate());
        m_writer.writeVersion();
        StateBase* state = doSerialize(value, 0);
        while (state)
            state = state->advance(*this);
        return m_status;
    }
