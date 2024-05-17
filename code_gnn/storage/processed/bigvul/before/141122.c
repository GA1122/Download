void Document::InitSecureContextState() {
  DCHECK_EQ(secure_context_state_, SecureContextState::kUnknown);
  if (!GetSecurityOrigin()->IsPotentiallyTrustworthy()) {
    secure_context_state_ = SecureContextState::kNonSecure;
  } else if (SchemeRegistry::SchemeShouldBypassSecureContextCheck(
                 GetSecurityOrigin()->Protocol())) {
    secure_context_state_ = SecureContextState::kSecure;
  } else if (frame_) {
    Frame* parent = frame_->Tree().Parent();
    while (parent) {
      if (!parent->GetSecurityContext()
               ->GetSecurityOrigin()
               ->IsPotentiallyTrustworthy()) {
        secure_context_state_ = SecureContextState::kNonSecure;
        break;
      }
      parent = parent->Tree().Parent();
    }
    if (secure_context_state_ == SecureContextState::kUnknown)
      secure_context_state_ = SecureContextState::kSecure;
  } else {
    secure_context_state_ = SecureContextState::kNonSecure;
  }
  DCHECK_NE(secure_context_state_, SecureContextState::kUnknown);
}
