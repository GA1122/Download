void WebContentsImpl::NotifySwapped(RenderViewHost* old_host,
                                    RenderViewHost* new_host) {
  notify_disconnection_ = true;
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    RenderViewHostChanged(old_host, new_host));

  std::pair<RenderViewHost*, RenderViewHost*> details =
      std::make_pair(old_host, new_host);
  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_VIEW_HOST_CHANGED,
      Source<WebContents>(this),
      Details<std::pair<RenderViewHost*, RenderViewHost*> >(&details));

  RemoveBrowserPluginEmbedder();
}
