void WebContentsImpl::OnWebUISend(RenderViewHostImpl* source,
                                  const GURL& source_url,
                                  const std::string& name,
                                  const base::ListValue& args) {
  if (delegate_)
    delegate_->WebUISend(this, source_url, name, args);
}
