void RenderViewImpl::SyncSelectionIfRequired() {
  WebFrame* frame = webview()->focusedFrame();
  if (!frame)
    return;

  string16 text;
  size_t offset;
  ui::Range range;
  if (pepper_helper_->IsPluginFocused()) {
    pepper_helper_->GetSurroundingText(&text, &range);
    offset = 0;   
  } else {
    size_t location, length;
    if (!webview()->caretOrSelectionRange(&location, &length))
      return;

    range = ui::Range(location, location + length);

    if (webview()->textInputType() != WebKit::WebTextInputTypeNone) {
      if (location > kExtraCharsBeforeAndAfterSelection)
        offset = location - kExtraCharsBeforeAndAfterSelection;
      else
        offset = 0;
      length = location + length - offset + kExtraCharsBeforeAndAfterSelection;
      WebRange webrange = WebRange::fromDocumentRange(frame, offset, length);
      if (!webrange.isNull())
        text = WebRange::fromDocumentRange(frame, offset, length).toPlainText();
    } else {
      offset = location;
      text = frame->selectionAsText();
      range.set_end(range.start() + text.length());
    }
  }

  if (selection_text_offset_ != offset ||
      selection_range_ != range ||
      selection_text_ != text) {
    selection_text_ = text;
    selection_text_offset_ = offset;
    selection_range_ = range;
    Send(new ViewHostMsg_SelectionChanged(routing_id_, text, offset, range));
    UpdateTextInputState(SHOW_IME_IF_NEEDED);
  }
}
