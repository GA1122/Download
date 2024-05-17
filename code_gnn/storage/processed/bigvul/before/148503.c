void WebContentsImpl::OnRequestPpapiBrokerPermission(
    RenderViewHostImpl* source,
    int ppb_broker_route_id,
    const GURL& url,
    const base::FilePath& plugin_path) {
  base::Callback<void(bool)> permission_result_callback = base::Bind(
      &WebContentsImpl::SendPpapiBrokerPermissionResult, base::Unretained(this),
      source->GetProcess()->GetID(), ppb_broker_route_id);
  if (!delegate_) {
    permission_result_callback.Run(false);
    return;
  }

  if (!delegate_->RequestPpapiBrokerPermission(this, url, plugin_path,
                                               permission_result_callback)) {
    NOTIMPLEMENTED();
    permission_result_callback.Run(false);
  }
}
