GURL GetOriginalRequestURL(WebDocumentLoader* document_loader) {
  GURL overriden_url;
  if (MaybeGetOverriddenURL(document_loader, &overriden_url))
    return overriden_url;

  std::vector<GURL> redirects;
  GetRedirectChain(document_loader, &redirects);
  if (!redirects.empty())
    return redirects.at(0);

  return document_loader->OriginalUrl();
}
