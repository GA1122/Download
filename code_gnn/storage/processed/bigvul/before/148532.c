void WebContentsImpl::RenderViewReady(RenderViewHost* rvh) {
  if (rvh != GetRenderViewHost()) {
    return;
  }

  RenderWidgetHostViewBase* rwhv =
      static_cast<RenderWidgetHostViewBase*>(GetRenderWidgetHostView());
  if (rwhv)
    rwhv->SetMainFrameAXTreeID(GetMainFrame()->GetAXTreeID());

  notify_disconnection_ = true;
  NotificationService::current()->Notify(
      NOTIFICATION_WEB_CONTENTS_CONNECTED,
      Source<WebContents>(this),
      NotificationService::NoDetails());

  bool was_crashed = IsCrashed();
  SetIsCrashed(base::TERMINATION_STATUS_STILL_RUNNING, 0);

  if (was_crashed && !FocusLocationBarByDefault() &&
      (!delegate_ || delegate_->ShouldFocusPageAfterCrash())) {
    view_->Focus();
  }

  for (auto& observer : observers_)
    observer.RenderViewReady();
}
