    void setIntOnMap(v8::Local<v8::Map> map, const String16& key, int64_t value)
    {
        v8::Local<v8::String> v8Key = toV8String(m_isolate, key);
        if (!map->Set(m_context, v8Key, v8::Integer::New(m_isolate, value)).ToLocal(&map))
            return;
    }
