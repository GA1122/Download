    void setDoubleOnMap(v8::Local<v8::Map> map, const String16& key, double value)
    {
        v8::Local<v8::String> v8Key = toV8String(m_isolate, key);
        if (!map->Set(m_context, v8Key, v8::Number::New(m_isolate, value)).ToLocal(&map))
            return;
    }
