void Document::registerForPageCacheSuspensionCallbacks(Element* e)
{
    m_documentSuspensionCallbackElements.add(e);
}
