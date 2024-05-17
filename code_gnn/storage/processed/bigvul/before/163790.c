void DelegatedFrameHost::SubmitCompositorFrame(
    const viz::LocalSurfaceId& local_surface_id,
    cc::CompositorFrame frame) {
#if defined(OS_CHROMEOS)
  DCHECK(!resize_lock_ || !client_->IsAutoResizeEnabled());
#endif
  float frame_device_scale_factor = frame.metadata.device_scale_factor;
  viz::BeginFrameAck ack(frame.metadata.begin_frame_ack);

  DCHECK(!frame.render_pass_list.empty());

  cc::RenderPass* root_pass = frame.render_pass_list.back().get();

  gfx::Size frame_size = root_pass->output_rect.size();
  gfx::Size frame_size_in_dip =
      gfx::ConvertSizeToDIP(frame_device_scale_factor, frame_size);

  gfx::Rect damage_rect = root_pass->damage_rect;
  damage_rect.Intersect(gfx::Rect(frame_size));
  gfx::Rect damage_rect_in_dip =
      gfx::ConvertRectToDIP(frame_device_scale_factor, damage_rect);

  if (ShouldSkipFrame(frame_size_in_dip)) {
    std::vector<viz::ReturnedResource> resources =
        viz::TransferableResource::ReturnResources(frame.resource_list);

    skipped_latency_info_list_.insert(skipped_latency_info_list_.end(),
                                      frame.metadata.latency_info.begin(),
                                      frame.metadata.latency_info.end());

    renderer_compositor_frame_sink_->DidReceiveCompositorFrameAck(resources);

    skipped_frames_ = true;
    ack.has_damage = false;
    DidNotProduceFrame(ack);
    return;
  }

  allow_one_renderer_frame_during_resize_lock_ = false;

  if (skipped_frames_) {
    skipped_frames_ = false;
    damage_rect = gfx::Rect(frame_size);
    damage_rect_in_dip = gfx::Rect(frame_size_in_dip);

    cc::RenderPass* root_pass = frame.render_pass_list.back().get();
    root_pass->damage_rect = damage_rect;
  }

  background_color_ = frame.metadata.root_background_color;

  if (frame_size.IsEmpty()) {
    DCHECK(frame.resource_list.empty());
    EvictDelegatedFrame();
  } else {
    ImageTransportFactory* factory = ImageTransportFactory::GetInstance();
    viz::FrameSinkManagerImpl* manager =
        factory->GetContextFactoryPrivate()->GetFrameSinkManager();

    frame.metadata.latency_info.insert(frame.metadata.latency_info.end(),
                                       skipped_latency_info_list_.begin(),
                                       skipped_latency_info_list_.end());
    skipped_latency_info_list_.clear();

    bool result =
        support_->SubmitCompositorFrame(local_surface_id, std::move(frame));
    DCHECK(result);

    if (local_surface_id != local_surface_id_ || !has_frame_) {
      viz::SurfaceId surface_id(frame_sink_id_, local_surface_id);
      viz::SurfaceInfo surface_info(surface_id, frame_device_scale_factor,
                                    frame_size);
      client_->DelegatedFrameHostGetLayer()->SetShowPrimarySurface(
          surface_info, manager->surface_manager()->reference_factory());
      client_->DelegatedFrameHostGetLayer()->SetFallbackSurface(surface_info);
      current_surface_size_ = frame_size;
      current_scale_factor_ = frame_device_scale_factor;
    }

    has_frame_ = true;
  }
  local_surface_id_ = local_surface_id;

  released_front_lock_ = NULL;
  current_frame_size_in_dip_ = frame_size_in_dip;
  CheckResizeLock();

  UpdateGutters();

  if (!damage_rect_in_dip.IsEmpty()) {
    client_->DelegatedFrameHostGetLayer()->OnDelegatedFrameDamage(
        damage_rect_in_dip);
  }

  if (has_frame_) {
    frame_evictor_->SwappedFrame(client_->DelegatedFrameHostIsVisible());
  }
 }
