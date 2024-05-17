    void set(const v8::Handle<GCObject>& handle, const T& value)
    {
        m_map.set(*handle, value);
    }
