    void openComposite(const v8::Local<v8::Value>& object)
    {
        uint32_t newObjectReference = m_objectPool.size();
        m_openCompositeReferenceStack.append(newObjectReference);
        m_objectPool.append(object);
    }
