void Layer::SetScrollOffsetFromImplSide(const gfx::Vector2d& scroll_offset) {
  DCHECK(IsPropertyChangeAllowed());
  DCHECK(layer_tree_host_ && layer_tree_host_->CommitRequested());
  if (scroll_offset_ == scroll_offset)
    return;
  scroll_offset_ = scroll_offset;
  SetNeedsPushProperties();
  if (!did_scroll_callback_.is_null())
    did_scroll_callback_.Run();
}
