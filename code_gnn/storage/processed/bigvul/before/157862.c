void WebContentsImpl::SetNotWaitingForResponse() {
  if (waiting_for_response_ == false)
    return;

  waiting_for_response_ = false;
  if (delegate_)
    delegate_->LoadingStateChanged(this, is_load_to_different_document_);
  for (auto& observer : observers_)
    observer.DidReceiveResponse();
}
