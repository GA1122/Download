void RenderFrameImpl::OnImeSetComposition(
    const base::string16& text,
    const std::vector<blink::WebImeTextSpan>& ime_text_spans,
    int selection_start,
    int selection_end) {
  if (!IsPepperAcceptingCompositionEvents()) {
    pepper_composition_text_ = text;
  } else {

    if (pepper_composition_text_.empty() && !text.empty()) {
      focused_pepper_plugin_->HandleCompositionStart(base::string16());
    }
    if (!pepper_composition_text_.empty() && text.empty()) {
      focused_pepper_plugin_->HandleCompositionEnd(base::string16());
    }
    pepper_composition_text_ = text;
    if (!pepper_composition_text_.empty()) {
      focused_pepper_plugin_->HandleCompositionUpdate(
          pepper_composition_text_, ime_text_spans, selection_start,
          selection_end);
    }
  }
}
