void RenderViewHostImpl::OnDidChangeScrollOffsetPinningForMainFrame(
    bool is_pinned_to_left, bool is_pinned_to_right) {
  if (view_)
    view_->SetScrollOffsetPinning(is_pinned_to_left, is_pinned_to_right);
}
