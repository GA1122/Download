const KURL& FrameFetchContext::Url() const {
  if (IsDetached())
    return frozen_state_->url;
  if (!document_)
    return NullURL();
  return document_->Url();
}
