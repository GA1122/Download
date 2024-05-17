ReferrerPolicy FrameFetchContext::GetReferrerPolicy() const {
  if (IsDetached())
    return frozen_state_->referrer_policy;
  return document_->GetReferrerPolicy();
}
