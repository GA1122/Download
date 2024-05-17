void Document::documentWillSuspendForPageCache()
{
    documentWillBecomeInactive();

    HashSet<Element*>::iterator end = m_documentSuspensionCallbackElements.end();
    for (HashSet<Element*>::iterator i = m_documentSuspensionCallbackElements.begin(); i != end; ++i)
        (*i)->documentWillSuspendForPageCache();

#ifndef NDEBUG
    m_didDispatchViewportPropertiesChanged = false;
#endif
}
