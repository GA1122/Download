void WebPageProxy::didChangeCompositionSelection(bool hasComposition)
{
    m_pageClient->compositionSelectionChanged(hasComposition);
}
