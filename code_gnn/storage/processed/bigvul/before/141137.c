bool Document::IsSecureContext() const {
  bool is_secure = secure_context_state_ == SecureContextState::kSecure;
  if (GetSandboxFlags() != kSandboxNone) {
    UseCounter::Count(
        *this, is_secure
                   ? WebFeature::kSecureContextCheckForSandboxedOriginPassed
                   : WebFeature::kSecureContextCheckForSandboxedOriginFailed);
  }
  UseCounter::Count(*this, is_secure ? WebFeature::kSecureContextCheckPassed
                                     : WebFeature::kSecureContextCheckFailed);
  return is_secure;
}
