void RenderFrameImpl::OnAdjustSelectionByCharacterOffset(int start_adjust,
                                                         int end_adjust) {
  size_t start, length;
  if (!GetRenderWidget()->webwidget()->caretOrSelectionRange(
      &start, &length)) {
    return;
  }

  if (start_adjust - end_adjust > static_cast<int>(length)
      || static_cast<int>(start) + start_adjust < 0) {
    return;
  }

  start += start_adjust;
  length += end_adjust - start_adjust;

  frame_->selectRange(WebRange::fromDocumentRange(frame_, start, length));
}
