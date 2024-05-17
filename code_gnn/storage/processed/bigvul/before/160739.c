void RenderFrameImpl::SyncSelectionIfRequired() {
  base::string16 text;
  size_t offset;
  gfx::Range range;
#if BUILDFLAG(ENABLE_PLUGINS)
  if (focused_pepper_plugin_) {
    focused_pepper_plugin_->GetSurroundingText(&text, &range);
    offset = 0;   
  } else
#endif
  {
    WebRange selection =
        frame_->GetInputMethodController()->GetSelectionOffsets();
    if (selection.IsNull())
      return;

    range = gfx::Range(selection.StartOffset(), selection.EndOffset());

    if (frame_->GetInputMethodController()->TextInputType() !=
        blink::kWebTextInputTypeNone) {
      if (selection.StartOffset() > kExtraCharsBeforeAndAfterSelection)
        offset = selection.StartOffset() - kExtraCharsBeforeAndAfterSelection;
      else
        offset = 0;
      size_t length =
          selection.EndOffset() - offset + kExtraCharsBeforeAndAfterSelection;
      text = frame_->RangeAsText(WebRange(offset, length)).Utf16();
    } else {
      offset = selection.StartOffset();
      text = frame_->SelectionAsText().Utf16();
      range.set_end(range.start() + text.length());
    }
  }

  if (selection_text_offset_ != offset ||
      selection_range_ != range ||
      selection_text_ != text) {
    selection_text_ = text;
    selection_text_offset_ = offset;
    selection_range_ = range;
    SetSelectedText(text, offset, range);
  }
  GetRenderWidget()->UpdateSelectionBounds();
}
