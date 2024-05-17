    v8::MaybeLocal<v8::Map> privateMap(const char* name)
    {
        v8::Local<v8::Object> console = ensureConsole();
        v8::Local<v8::Private> privateKey = v8::Private::ForApi(m_isolate, toV8StringInternalized(m_isolate, name));
        v8::Local<v8::Value> mapValue;
        if (!console->GetPrivate(m_context, privateKey).ToLocal(&mapValue))
            return v8::MaybeLocal<v8::Map>();
        if (mapValue->IsUndefined()) {
            v8::Local<v8::Map> map = v8::Map::New(m_isolate);
            if (!console->SetPrivate(m_context, privateKey, map).FromMaybe(false))
                return v8::MaybeLocal<v8::Map>();
            return map;
        }
        return mapValue->IsMap() ? mapValue.As<v8::Map>() : v8::MaybeLocal<v8::Map>();
    }
