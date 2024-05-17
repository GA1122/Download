void RenderWidgetHostViewAura::EvictDelegatedFrame() {
  window_->layer()->SetShowPaintedContent();
  frame_provider_ = NULL;
  delegated_frame_evictor_->DiscardedFrame();
}
