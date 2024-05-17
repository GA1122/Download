 void DisplayItemList::beginScope()
{
    ASSERT_WITH_SECURITY_IMPLICATION(m_nextScope < UINT_MAX);
    m_scopeStack.append(m_nextScope++);
    beginSkippingCache();
}
