WebString WebLocalFrameImpl::RangeAsText(const WebRange& web_range) {
  GetFrame()->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  DocumentLifecycle::DisallowTransitionScope disallow_transition(
      GetFrame()->GetDocument()->Lifecycle());

  return PlainText(
      web_range.CreateEphemeralRange(GetFrame()),
      TextIteratorBehavior::EmitsObjectReplacementCharacterBehavior());
}
