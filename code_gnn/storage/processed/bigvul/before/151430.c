SecurityOrigin* FrameFetchContext::GetSecurityOrigin() const {
  if (IsDetached())
    return frozen_state_->security_origin.get();
  return document_ ? document_->GetSecurityOrigin() : nullptr;
}
