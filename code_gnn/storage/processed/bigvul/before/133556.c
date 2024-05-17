void WebContentsImpl::DocumentOnLoadCompletedInMainFrame(
    RenderViewHost* render_view_host,
    int32 page_id) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DocumentOnLoadCompletedInMainFrame(page_id));

  NotificationService::current()->Notify(
      NOTIFICATION_LOAD_COMPLETED_MAIN_FRAME,
      Source<WebContents>(this),
      Details<int>(&page_id));
}