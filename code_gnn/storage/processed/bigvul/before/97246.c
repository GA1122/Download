String WebFrameLoaderClient::userAgent(const KURL& url) {
  return webkit_glue::StdStringToString(
      webkit_glue::GetUserAgent(webkit_glue::KURLToGURL(url)));
}
