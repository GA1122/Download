    bool closeComposite(v8::Handle<v8::Value>* object)
    {
        if (!m_openCompositeReferenceStack.size())
            return false;
        uint32_t objectReference = m_openCompositeReferenceStack[m_openCompositeReferenceStack.size() - 1];
        m_openCompositeReferenceStack.shrink(m_openCompositeReferenceStack.size() - 1);
        if (objectReference >= m_objectPool.size())
            return false;
        *object = m_objectPool[objectReference];
        return true;
    }
