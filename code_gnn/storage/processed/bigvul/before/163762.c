DelegatedFrameHost::DelegatedFrameHost(const viz::FrameSinkId& frame_sink_id,
                                       DelegatedFrameHostClient* client)
    : frame_sink_id_(frame_sink_id),
      client_(client),
      compositor_(nullptr),
      tick_clock_(new base::DefaultTickClock()),
      skipped_frames_(false),
      background_color_(SK_ColorRED),
      current_scale_factor_(1.f),
      frame_evictor_(new viz::FrameEvictor(this)) {
  ImageTransportFactory* factory = ImageTransportFactory::GetInstance();
  factory->GetContextFactory()->AddObserver(this);
  viz::HostFrameSinkManager* host_frame_sink_manager =
      factory->GetContextFactoryPrivate()->GetHostFrameSinkManager();
  host_frame_sink_manager->RegisterFrameSinkId(frame_sink_id_, this);
  CreateCompositorFrameSinkSupport();
}
