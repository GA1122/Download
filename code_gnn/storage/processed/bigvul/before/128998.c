    StateBase* startObjectState(v8::Handle<v8::Object> object, StateBase* next)
    {
        m_writer.writeGenerateFreshObject();
        return push(new ObjectState(object, next));
    }
