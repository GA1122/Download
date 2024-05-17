    v8::Local<v8::Value> element(unsigned index)
    {
        ASSERT_WITH_SECURITY_IMPLICATION(index < m_stack.size());
        return m_stack[index];
    }
