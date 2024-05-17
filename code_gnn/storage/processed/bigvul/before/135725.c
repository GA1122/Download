String InputMethodController::ComposingText() const {
  DocumentLifecycle::DisallowTransitionScope disallow_transition(
      GetDocument().Lifecycle());
  return PlainText(
      CompositionEphemeralRange(),
      TextIteratorBehavior::Builder().SetEmitsOriginalText(true).Build());
}
