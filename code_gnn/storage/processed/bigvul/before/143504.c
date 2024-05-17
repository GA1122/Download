viz::HostFrameSinkManager* CompositorImpl::GetHostFrameSinkManager() {
  return &CompositorDependencies::Get().host_frame_sink_manager;
}
