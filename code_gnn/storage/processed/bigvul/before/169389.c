WebContentsAddedObserver::WebContentsAddedObserver()
    : web_contents_created_callback_(
          base::Bind(&WebContentsAddedObserver::WebContentsCreated,
                     base::Unretained(this))),
      web_contents_(nullptr) {
  WebContentsImpl::FriendWrapper::AddCreatedCallbackForTesting(
      web_contents_created_callback_);
}
