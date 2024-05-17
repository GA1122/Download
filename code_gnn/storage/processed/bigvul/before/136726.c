String FrameSelection::SelectedTextForClipboard() const {
  return ExtractSelectedText(
      *this, TextIteratorBehavior::Builder()
                 .SetEmitsImageAltText(
                     frame_->GetSettings() &&
                     frame_->GetSettings()->GetSelectionIncludesAltImageText())
                 .SetSkipsUnselectableContent(true)
                 .Build());
}
