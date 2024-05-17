void SynchronizeVisualPropertiesMessageFilter::OnUpdatedFrameSinkIdOnUI() {
  frame_sink_id_run_loop_.Quit();
}
