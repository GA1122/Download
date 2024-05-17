void WebPage::download(const Platform::NetworkRequest& request)
{
    vector<const char*> headers;
    Platform::NetworkRequest::HeaderList& list = request.getHeaderListRef();
    for (unsigned i = 0; i < list.size(); i++) {
        headers.push_back(list[i].first.c_str());
        headers.push_back(list[i].second.c_str());
    }
    d->load(request.getUrlRef(), BlackBerry::Platform::String::emptyString(), "GET", Platform::NetworkRequest::UseProtocolCachePolicy, 0, 0, headers.empty() ? 0 : &headers[0], headers.size(), false, false, true, "", request.getSuggestedSaveName().c_str());
}
