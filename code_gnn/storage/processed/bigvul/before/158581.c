void WebLocalFrameImpl::SetTextDirection(WebTextDirection direction) {
  Editor& editor = frame_->GetEditor();
  if (!editor.CanEdit())
    return;

  switch (direction) {
    case kWebTextDirectionDefault:
      editor.SetBaseWritingDirection(WritingDirection::kNatural);
      break;

    case kWebTextDirectionLeftToRight:
      editor.SetBaseWritingDirection(WritingDirection::kLeftToRight);
      break;

    case kWebTextDirectionRightToLeft:
      editor.SetBaseWritingDirection(WritingDirection::kRightToLeft);
      break;

    default:
      NOTIMPLEMENTED();
      break;
  }
}
