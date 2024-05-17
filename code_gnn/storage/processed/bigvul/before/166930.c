bool BaseAudioContext::IsAllowedToStart() const {
  if (!user_gesture_required_)
    return true;

  Document* document = ToDocument(GetExecutionContext());
  DCHECK(document);

  switch (GetAutoplayPolicy()) {
    case AutoplayPolicy::Type::kNoUserGestureRequired:
      NOTREACHED();
      break;
    case AutoplayPolicy::Type::kUserGestureRequired:
    case AutoplayPolicy::Type::kUserGestureRequiredForCrossOrigin:
      DCHECK(document->GetFrame() &&
             document->GetFrame()->IsCrossOriginSubframe());
      document->AddConsoleMessage(ConsoleMessage::Create(
          kOtherMessageSource, kWarningMessageLevel,
          "The AudioContext was not allowed to start. It must be resumed (or "
          "created) from a user gesture event handler."));
      break;
    case AutoplayPolicy::Type::kDocumentUserActivationRequired:
      document->AddConsoleMessage(ConsoleMessage::Create(
          kOtherMessageSource, kWarningMessageLevel,
          "The AudioContext was not allowed to start. It must be resume (or "
          "created) after a user gesture on the page. https://goo.gl/7K7WLu"));
      break;
  }

  return false;
}
