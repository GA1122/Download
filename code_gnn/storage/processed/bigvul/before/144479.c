void WebContentsImpl::DidGetRedirectForResourceRequest(
  RenderFrameHost* render_frame_host,
  const ResourceRedirectDetails& details) {
  controller_.ssl_manager()->DidReceiveResourceRedirect(details);

  FOR_EACH_OBSERVER(
      WebContentsObserver,
      observers_,
      DidGetRedirectForResourceRequest(render_frame_host, details));

  NotificationService::current()->Notify(
      NOTIFICATION_RESOURCE_RECEIVED_REDIRECT,
      Source<WebContents>(this),
      Details<const ResourceRedirectDetails>(&details));
}