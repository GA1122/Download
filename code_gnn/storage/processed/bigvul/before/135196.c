WeakPtrWillBeRawPtr<Document> Document::createWeakPtr()
{
#if ENABLE(OILPAN)
    return this;
#else
    return m_weakFactory.createWeakPtr();
#endif
}
