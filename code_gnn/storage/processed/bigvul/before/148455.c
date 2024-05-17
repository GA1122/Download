void WebContentsImpl::NotifySwappedFromRenderManager(RenderFrameHost* old_host,
                                                     RenderFrameHost* new_host,
                                                     bool is_main_frame) {
  if (is_main_frame) {
    NotifyViewSwapped(old_host ? old_host->GetRenderViewHost() : nullptr,
                      new_host->GetRenderViewHost());

    if (delegate_)
      view_->SetOverscrollControllerEnabled(CanOverscrollContent());

    view_->RenderViewSwappedIn(new_host->GetRenderViewHost());

    RenderWidgetHostViewBase* rwhv =
        static_cast<RenderWidgetHostViewBase*>(GetRenderWidgetHostView());
    if (rwhv)
      rwhv->SetMainFrameAXTreeID(GetMainFrame()->GetAXTreeID());
  }

  NotifyFrameSwapped(old_host, new_host);
}
