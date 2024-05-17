AutoplayPolicy::Type BaseAudioContext::GetAutoplayPolicy() const {
  if (RuntimeEnabledFeatures::AutoplayIgnoresWebAudioEnabled()) {
#if defined(OS_ANDROID)
    return AutoplayPolicy::Type::kUserGestureRequired;
#else
    return AutoplayPolicy::Type::kNoUserGestureRequired;
#endif
  }

  Document* document = GetDocument();
  DCHECK(document);
  return AutoplayPolicy::GetAutoplayPolicyForDocument(*document);
}
