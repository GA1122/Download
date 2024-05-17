void WebPageProxy::didFinishLoadingDataForCustomRepresentation(const String& suggestedFilename, const CoreIPC::DataReference& dataReference)
{
    m_pageClient->didFinishLoadingDataForCustomRepresentation(suggestedFilename, dataReference);
}
