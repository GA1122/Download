void LayerTreeHost::SetFrameSinkId(const FrameSinkId& frame_sink_id) {
  surface_sequence_generator_.set_frame_sink_id(frame_sink_id);
}
