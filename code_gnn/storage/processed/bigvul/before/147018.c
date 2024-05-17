bool WebLocalFrameImpl::IsSpellCheckingEnabled() const {
  return GetFrame()->GetSpellChecker().IsSpellCheckingEnabled();
}
