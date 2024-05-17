void InputConnectionImpl::SetComposingText(
    const base::string16& text,
    int new_cursor_pos,
    const base::Optional<gfx::Range>& new_selection_range) {
  new_cursor_pos += text.length() - 1;

  StartStateUpdateTimer();

  const int selection_start = new_selection_range
                                  ? new_selection_range.value().start()
                                  : new_cursor_pos;
  const int selection_end =
      new_selection_range ? new_selection_range.value().end() : new_cursor_pos;

  std::string error;
  if (!ime_engine_->SetComposition(
          input_context_id_, base::UTF16ToUTF8(text).c_str(), selection_start,
          selection_end, new_cursor_pos,
          std::vector<input_method::InputMethodEngineBase::SegmentInfo>(),
          &error)) {
    LOG(ERROR) << "SetComposingText failed: pos=" << new_cursor_pos
               << ", error=\"" << error << "\"";
    return;
  }
  composing_text_ = text;
}
