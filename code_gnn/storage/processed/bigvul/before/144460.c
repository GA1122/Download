WebUI* WebContentsImpl::CreateSubframeWebUI(const GURL& url,
                                            const std::string& frame_name) {
  DCHECK(!frame_name.empty());
  return CreateWebUI(url, frame_name);
}
