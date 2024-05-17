    bool readStringObject(v8::Handle<v8::Value>* value)
    {
        v8::Handle<v8::Value> stringValue;
        if (!readString(&stringValue) || !stringValue->IsString())
            return false;
        *value = v8::StringObject::New(stringValue.As<v8::String>());
        return true;
    }
