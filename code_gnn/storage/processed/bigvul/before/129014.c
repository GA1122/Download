    void writeBooleanObject(v8::Handle<v8::Value> value)
    {
        v8::Handle<v8::BooleanObject> booleanObject = value.As<v8::BooleanObject>();
        m_writer.writeBooleanObject(booleanObject->ValueOf());
    }
