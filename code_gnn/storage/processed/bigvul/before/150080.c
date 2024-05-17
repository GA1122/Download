void LayerTreeHostImpl::WillBeginImplFrame(const BeginFrameArgs& args) {
  current_begin_frame_tracker_.Start(args);

  if (is_likely_to_require_a_draw_) {
    SetNeedsRedraw();
  }

  if (input_handler_client_)
    input_handler_client_->DeliverInputForBeginFrame();

  Animate();

  for (auto* it : video_frame_controllers_)
    it->OnBeginFrame(args);
}
