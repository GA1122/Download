void RenderFrameImpl::HandlePepperImeCommit(const base::string16& text) {
  if (text.empty())
    return;

  if (!IsPepperAcceptingCompositionEvents()) {
    base::i18n::UTF16CharIterator iterator(&text);
    int32_t i = 0;
    while (iterator.Advance()) {
      blink::WebKeyboardEvent char_event(blink::WebInputEvent::kChar,
                                         blink::WebInputEvent::kNoModifiers,
                                         ui::EventTimeForNow());
      char_event.windows_key_code = text[i];
      char_event.native_key_code = text[i];

      const int32_t char_start = i;
      for (; i < iterator.array_pos(); ++i) {
        char_event.text[i - char_start] = text[i];
        char_event.unmodified_text[i - char_start] = text[i];
      }

      if (GetLocalRootRenderWidget()->GetWebWidget())
        GetLocalRootRenderWidget()->GetWebWidget()->HandleInputEvent(
            blink::WebCoalescedInputEvent(char_event));
    }
  } else {
    focused_pepper_plugin_->HandleCompositionEnd(text);
    focused_pepper_plugin_->HandleTextInput(text);
  }
  pepper_composition_text_.clear();
}
