void RenderViewHostImpl::OnStartContentIntent(const GURL& content_url) {
  if (GetView())
    GetView()->StartContentIntent(content_url);
}
