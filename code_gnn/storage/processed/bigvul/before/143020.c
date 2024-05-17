void BaseAudioContext::RejectPendingResolvers() {
  DCHECK(IsMainThread());


  for (auto& resolver : resume_resolvers_) {
    resolver->Reject(DOMException::Create(DOMExceptionCode::kInvalidStateError,
                                          "Audio context is going away"));
  }
  resume_resolvers_.clear();
  is_resolving_resume_promises_ = false;

  RejectPendingDecodeAudioDataResolvers();
}
