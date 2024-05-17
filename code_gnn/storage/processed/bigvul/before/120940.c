int SocketStream::DoResolveProxyComplete(int result) {
  pac_request_ = NULL;
  if (result != OK) {
    DVLOG(1) << "Failed to resolve proxy: " << result;
    if (delegate_)
      delegate_->OnError(this, result);
    proxy_info_.UseDirect();
  }
  if (proxy_info_.is_direct()) {
    if (!proxy_url_.SchemeIs("https")) {
      const std::string scheme = "https";
      GURL::Replacements repl;
      repl.SetSchemeStr(scheme);
      proxy_url_ = url_.ReplaceComponents(repl);
      DVLOG(1) << "Try https proxy: " << proxy_url_;
      next_state_ = STATE_RESOLVE_PROXY;
      return OK;
    }
  }

  if (proxy_info_.is_empty()) {
    return ERR_NO_SUPPORTED_PROXIES;
  }

  next_state_ = STATE_RESOLVE_HOST;
  return OK;
}
