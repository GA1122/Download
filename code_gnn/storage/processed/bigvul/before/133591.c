void WebContentsImpl::NotifySwappedFromRenderManager(RenderViewHost* old_host,
                                                     RenderViewHost* new_host) {
  NotifySwapped(old_host, new_host);

  if (delegate_)
    view_->SetOverscrollControllerEnabled(delegate_->CanOverscrollContent());

  view_->RenderViewSwappedIn(new_host);
}
