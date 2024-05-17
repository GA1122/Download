WebURLRequest::RequestContext ResourceFetcher::DetermineRequestContext(
    Resource::Type type,
    IsImageSet is_image_set,
    bool is_main_frame) {
  DCHECK((is_image_set == kImageNotImageSet) ||
         (type == Resource::kImage && is_image_set == kImageIsImageSet));
  switch (type) {
    case Resource::kMainResource:
      if (!is_main_frame)
        return WebURLRequest::kRequestContextIframe;
      return WebURLRequest::kRequestContextHyperlink;
    case Resource::kXSLStyleSheet:
      DCHECK(RuntimeEnabledFeatures::XSLTEnabled());
    case Resource::kCSSStyleSheet:
      return WebURLRequest::kRequestContextStyle;
    case Resource::kScript:
      return WebURLRequest::kRequestContextScript;
    case Resource::kFont:
      return WebURLRequest::kRequestContextFont;
    case Resource::kImage:
      if (is_image_set == kImageIsImageSet)
        return WebURLRequest::kRequestContextImageSet;
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
