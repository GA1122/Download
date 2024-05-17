WebURLRequest::RequestContext ResourceFetcher::DetermineRequestContext(
    Resource::Type type,
    bool is_main_frame) {
  switch (type) {
    case Resource::kMainResource:
      if (!is_main_frame)
        return WebURLRequest::kRequestContextIframe;
      return WebURLRequest::kRequestContextHyperlink;
    case Resource::kXSLStyleSheet:
      DCHECK(RuntimeEnabledFeatures::xsltEnabled());
    case Resource::kCSSStyleSheet:
      return WebURLRequest::kRequestContextStyle;
    case Resource::kScript:
      return WebURLRequest::kRequestContextScript;
    case Resource::kFont:
      return WebURLRequest::kRequestContextFont;
    case Resource::kImage:
      return WebURLRequest::kRequestContextImage;
    case Resource::kRaw:
      return WebURLRequest::kRequestContextSubresource;
    case Resource::kImportResource:
      return WebURLRequest::kRequestContextImport;
    case Resource::kLinkPrefetch:
      return WebURLRequest::kRequestContextPrefetch;
    case Resource::kTextTrack:
      return WebURLRequest::kRequestContextTrack;
    case Resource::kSVGDocument:
      return WebURLRequest::kRequestContextImage;
    case Resource::kMedia:   
      return WebURLRequest::kRequestContextVideo;
    case Resource::kManifest:
      return WebURLRequest::kRequestContextManifest;
    case Resource::kMock:
      return WebURLRequest::kRequestContextSubresource;
  }
  NOTREACHED();
  return WebURLRequest::kRequestContextSubresource;
}
