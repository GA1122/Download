viz::FrameSinkId CompositorImpl::AllocateFrameSinkId() {
  return CompositorDependencies::Get()
      .frame_sink_id_allocator.NextFrameSinkId();
}
