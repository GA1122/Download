void WebContentsImpl::SaveFrame(const GURL& url,
                                const Referrer& referrer) {
  SaveFrameWithHeaders(url, referrer, std::string());
}
