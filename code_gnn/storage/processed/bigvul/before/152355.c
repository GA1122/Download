void GetRedirectChain(WebDocumentLoader* document_loader,
                      std::vector<GURL>* result) {
  WebVector<WebURL> urls;
  document_loader->RedirectChain(urls);
  result->reserve(urls.size());
  for (size_t i = 0; i < urls.size(); ++i) {
    result->push_back(urls[i]);
  }
}
