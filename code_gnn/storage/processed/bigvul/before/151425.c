String FrameFetchContext::GetOutgoingReferrer() const {
  if (IsDetached())
    return frozen_state_->outgoing_referrer;
  return document_->OutgoingReferrer();
}
