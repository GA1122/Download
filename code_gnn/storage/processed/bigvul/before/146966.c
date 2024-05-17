void WebLocalFrameImpl::EnableSpellChecking(bool enable) {
  if (enable == IsSpellCheckingEnabled())
    return;
  GetFrame()->GetSpellChecker().ToggleSpellCheckingEnabled();
}
