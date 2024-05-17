SSLManager::SSLManager(NavigationControllerImpl* controller)
    : backend_(controller),
      policy_(new SSLPolicy(&backend_)),
      controller_(controller) {
  DCHECK(controller_);

  registrar_.Add(
      this, content::NOTIFICATION_RESOURCE_RESPONSE_STARTED,
      content::Source<WebContents>(controller_->web_contents()));
  registrar_.Add(
      this, content::NOTIFICATION_RESOURCE_RECEIVED_REDIRECT,
      content::Source<WebContents>(controller_->web_contents()));
  registrar_.Add(
      this, content::NOTIFICATION_LOAD_FROM_MEMORY_CACHE,
      content::Source<NavigationController>(controller_));
  registrar_.Add(
      this, content::NOTIFICATION_SSL_INTERNAL_STATE_CHANGED,
      content::Source<content::BrowserContext>(
          controller_->GetBrowserContext()));
}
