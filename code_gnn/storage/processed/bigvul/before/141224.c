void Document::SetContentLanguage(const AtomicString& language) {
  if (content_language_ == language)
    return;
  content_language_ = language;

  SetNeedsStyleRecalc(kSubtreeStyleChange, StyleChangeReasonForTracing::Create(
                                               style_change_reason::kLanguage));
}
