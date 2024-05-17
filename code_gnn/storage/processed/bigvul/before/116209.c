void SSLManager::Observe(int type,
                         const content::NotificationSource& source,
                         const content::NotificationDetails& details) {
  switch (type) {
    case content::NOTIFICATION_RESOURCE_RESPONSE_STARTED:
      DidStartResourceResponse(
          content::Details<ResourceRequestDetails>(details).ptr());
      break;
    case content::NOTIFICATION_RESOURCE_RECEIVED_REDIRECT:
      DidReceiveResourceRedirect(
          content::Details<ResourceRedirectDetails>(details).ptr());
      break;
    case content::NOTIFICATION_LOAD_FROM_MEMORY_CACHE:
      DidLoadFromMemoryCache(
          content::Details<LoadFromMemoryCacheDetails>(details).ptr());
      break;
    case content::NOTIFICATION_SSL_INTERNAL_STATE_CHANGED:
      DidChangeSSLInternalState();
      break;
    default:
      NOTREACHED() << "The SSLManager received an unexpected notification.";
  }
}
