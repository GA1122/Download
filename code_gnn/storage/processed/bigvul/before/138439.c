void Document::processHttpEquivRefresh(const AtomicString& content)
{
    maybeHandleHttpRefresh(content, HttpRefreshFromMetaTag);
}
