DelegatedFrameHost::~DelegatedFrameHost() {
  DCHECK(!compositor_);
  ImageTransportFactory* factory = ImageTransportFactory::GetInstance();
  factory->GetContextFactory()->RemoveObserver(this);

  ResetCompositorFrameSinkSupport();

  viz::HostFrameSinkManager* host_frame_sink_manager =
      factory->GetContextFactoryPrivate()->GetHostFrameSinkManager();
  host_frame_sink_manager->InvalidateFrameSinkId(frame_sink_id_);

  DCHECK(!vsync_manager_.get());
}
