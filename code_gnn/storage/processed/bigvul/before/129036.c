    void writeNumberObject(v8::Handle<v8::Value> value)
    {
        v8::Handle<v8::NumberObject> numberObject = value.As<v8::NumberObject>();
        m_writer.writeNumberObject(numberObject->ValueOf());
    }
