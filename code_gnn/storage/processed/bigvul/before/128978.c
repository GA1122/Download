    bool readInt32(v8::Handle<v8::Value>* value)
    {
        uint32_t rawValue;
        if (!doReadUint32(&rawValue))
            return false;
        *value = v8::Integer::New(isolate(), static_cast<int32_t>(ZigZag::decode(rawValue)));
        return true;
    }
