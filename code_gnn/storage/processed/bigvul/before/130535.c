void DisplayItemList::endScope()
{
    m_scopeStack.removeLast();
    endSkippingCache();
}
