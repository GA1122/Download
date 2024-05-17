viz::FrameSinkId SynchronizeVisualPropertiesMessageFilter::GetOrWaitForId() {
  frame_sink_id_run_loop_.Run();
  return frame_sink_id_;
}
