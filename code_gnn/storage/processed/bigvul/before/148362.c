void WebContentsImpl::DocumentOnLoadCompleted(
    RenderFrameHost* render_frame_host) {
  ShowInsecureLocalhostWarningIfNeeded();

  is_notifying_observers_ = true;
  for (auto& observer : observers_)
    observer.DocumentOnLoadCompletedInMainFrame();
  is_notifying_observers_ = false;

  NotificationService::current()->Notify(
      NOTIFICATION_LOAD_COMPLETED_MAIN_FRAME,
      Source<WebContents>(this),
      NotificationService::NoDetails());
}
