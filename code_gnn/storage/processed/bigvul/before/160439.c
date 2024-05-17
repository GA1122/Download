mojom::FrameInputHandler* RenderFrameHostImpl::GetFrameInputHandler() {
  if (legacy_frame_input_handler_)
    return legacy_frame_input_handler_.get();
  return frame_input_handler_.get();
}
