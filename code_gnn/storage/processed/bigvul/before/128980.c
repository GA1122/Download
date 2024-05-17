    bool readNumberObject(v8::Handle<v8::Value>* value)
    {
        double number;
        if (!doReadNumber(&number))
            return false;
        *value = v8::NumberObject::New(isolate(), number);
        return true;
    }
