WeakPtrWillBeRawPtr<Document> Document::contextDocument()
{
    if (m_contextDocument)
        return m_contextDocument;
    if (m_frame) {
#if ENABLE(OILPAN)
        return this;
#else
        return m_weakFactory.createWeakPtr();
#endif
    }
    return nullptr;
}
