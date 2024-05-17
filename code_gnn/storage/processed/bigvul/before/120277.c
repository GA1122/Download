void LayerTreeHost::StartPageScaleAnimation(const gfx::Vector2d& target_offset,
                                            bool use_anchor,
                                            float scale,
                                            base::TimeDelta duration) {
  pending_page_scale_animation_.reset(new PendingPageScaleAnimation);
  pending_page_scale_animation_->target_offset = target_offset;
  pending_page_scale_animation_->use_anchor = use_anchor;
  pending_page_scale_animation_->scale = scale;
  pending_page_scale_animation_->duration = duration;

  SetNeedsCommit();
}
