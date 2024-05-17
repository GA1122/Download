void WebContentsImpl::DocumentOnLoadCompleted(
    RenderFrameHost* render_frame_host) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DocumentOnLoadCompletedInMainFrame());

  NotificationService::current()->Notify(
      NOTIFICATION_LOAD_COMPLETED_MAIN_FRAME,
      Source<WebContents>(this),
      NotificationService::NoDetails());
}
