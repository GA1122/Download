WebCString WebPageSerializer::serializeToMHTML(WebView* view)
{
    RefPtr<SharedBuffer> mhtml = serializePageToMHTML(toWebViewImpl(view)->page(), MHTMLArchive::UseDefaultEncoding);
    return WebCString(mhtml->data(), mhtml->size());
}
