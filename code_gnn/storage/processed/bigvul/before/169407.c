WebContentsAddedObserver::~WebContentsAddedObserver() {
  WebContentsImpl::FriendWrapper::RemoveCreatedCallbackForTesting(
      web_contents_created_callback_);
}
