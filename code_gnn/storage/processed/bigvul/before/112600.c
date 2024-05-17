void Document::unregisterForPageCacheSuspensionCallbacks(Element* e)
{
    m_documentSuspensionCallbackElements.remove(e);
}
