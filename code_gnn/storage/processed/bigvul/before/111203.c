void WebPage::loadExtended(const char* url, const char* networkToken, const char* method, Platform::NetworkRequest::CachePolicy cachePolicy, const char* data, size_t dataLength, const char* const* headers, size_t headersLength, bool mustHandleInternally)
{
    d->load(url, networkToken, method, cachePolicy, data, dataLength, headers, headersLength, false, mustHandleInternally, false, "");
}
