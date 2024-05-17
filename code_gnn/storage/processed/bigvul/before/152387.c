bool RenderFrameImpl::IsLocalRoot() const {
  bool is_local_root = static_cast<bool>(render_widget_);
  DCHECK_EQ(is_local_root,
            !(frame_->Parent() && frame_->Parent()->IsWebLocalFrame()));
  return is_local_root;
}
