    bool readRegExp(v8::Handle<v8::Value>* value)
    {
        v8::Handle<v8::Value> pattern;
        if (!readString(&pattern))
            return false;
        uint32_t flags;
        if (!doReadUint32(&flags))
            return false;
        *value = v8::RegExp::New(pattern.As<v8::String>(), static_cast<v8::RegExp::Flags>(flags));
        return true;
    }
