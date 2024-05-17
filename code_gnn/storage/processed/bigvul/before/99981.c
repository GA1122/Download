GURL WebPluginImpl::CompleteURL(const char* url) {
  if (!webframe_) {
    NOTREACHED();
    return GURL();
  }
  return webframe_->completeURL(WebString::fromUTF8(url));
}
