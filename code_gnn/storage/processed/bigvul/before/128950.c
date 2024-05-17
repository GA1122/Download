    void greyObject(const v8::Handle<v8::Object>& object)
    {
        ASSERT(!m_objectPool.contains(object));
        uint32_t objectReference = m_nextObjectReference++;
        m_objectPool.set(object, objectReference);
    }
