void Document::processHttpEquivRefresh(const String& content)
{
    maybeHandleHttpRefresh(content, HttpRefreshFromMetaTag);
}
