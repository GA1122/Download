    bool writeCryptoKey(v8::Handle<v8::Value> value)
    {
        CryptoKey* key = V8CryptoKey::toNative(value.As<v8::Object>());
        if (!key)
            return false;
        return m_writer.writeCryptoKey(key->key());
    }
