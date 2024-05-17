bool BaseAudioContext::AreAutoplayRequirementsFulfilled() const {
  switch (GetAutoplayPolicy()) {
    case AutoplayPolicy::Type::kNoUserGestureRequired:
      return true;
    case AutoplayPolicy::Type::kUserGestureRequired:
    case AutoplayPolicy::Type::kUserGestureRequiredForCrossOrigin:
      return Frame::HasTransientUserActivation(
          GetDocument() ? GetDocument()->GetFrame() : nullptr);
    case AutoplayPolicy::Type::kDocumentUserActivationRequired:
      return AutoplayPolicy::IsDocumentAllowedToPlay(*GetDocument());
  }

  NOTREACHED();
  return false;
}
