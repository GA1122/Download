void DelegatedFrameHost::CreateCompositorFrameSinkSupport() {
  DCHECK(!support_);
  constexpr bool is_root = false;
  constexpr bool needs_sync_points = true;
  ImageTransportFactory* factory = ImageTransportFactory::GetInstance();
  support_ = factory->GetContextFactoryPrivate()
                 ->GetHostFrameSinkManager()
                 ->CreateCompositorFrameSinkSupport(this, frame_sink_id_,
                                                    is_root, needs_sync_points);
  if (compositor_)
    compositor_->AddFrameSink(frame_sink_id_);
  if (needs_begin_frame_)
    support_->SetNeedsBeginFrame(true);
}
