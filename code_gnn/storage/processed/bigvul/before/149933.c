void LayerTreeHostImpl::DidFinishImplFrame() {
  current_begin_frame_tracker_.Finish();
  decoded_image_tracker_.NotifyFrameFinished();
}
