WebKit::WebURLLoader* TestWebKitPlatformSupport::createURLLoader() {
  if (!unit_test_mode_)
    return webkit_glue::WebKitPlatformSupportImpl::createURLLoader();
  return url_loader_factory_.CreateURLLoader(
      webkit_glue::WebKitPlatformSupportImpl::createURLLoader());
}
