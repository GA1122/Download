void RenderWidgetHostViewAura::SwapDelegatedFrame(
    uint32 output_surface_id,
    scoped_ptr<cc::DelegatedFrameData> frame_data,
    float frame_device_scale_factor,
    const std::vector<ui::LatencyInfo>& latency_info) {
  DCHECK_NE(0u, frame_data->render_pass_list.size());

  cc::RenderPass* root_pass = frame_data->render_pass_list.back();

  gfx::Size frame_size = root_pass->output_rect.size();
  gfx::Size frame_size_in_dip =
      ConvertSizeToDIP(frame_device_scale_factor, frame_size);

  gfx::Rect damage_rect = gfx::ToEnclosingRect(root_pass->damage_rect);
  damage_rect.Intersect(gfx::Rect(frame_size));
  gfx::Rect damage_rect_in_dip =
      ConvertRectToDIP(frame_device_scale_factor, damage_rect);

  software_frame_manager_->DiscardCurrentFrame();

  if (ShouldSkipFrame(frame_size_in_dip)) {
    cc::CompositorFrameAck ack;
    cc::TransferableResource::ReturnResources(frame_data->resource_list,
                                              &ack.resources);
    RenderWidgetHostImpl::SendSwapCompositorFrameAck(
        host_->GetRoutingID(), output_surface_id,
        host_->GetProcess()->GetID(), ack);
    skipped_frames_ = true;
    return;
  }

  if (skipped_frames_) {
    skipped_frames_ = false;
    damage_rect = gfx::Rect(frame_size);
    damage_rect_in_dip = gfx::Rect(frame_size_in_dip);

    cc::RenderPass* root_pass = frame_data->render_pass_list.back();
    root_pass->damage_rect = damage_rect;
  }

  if (output_surface_id != last_output_surface_id_) {
    EvictDelegatedFrame();

    if (resource_collection_.get()) {
      resource_collection_->SetClient(NULL);

      if (resource_collection_->LoseAllResources())
        SendReturnedDelegatedResources(last_output_surface_id_);

      resource_collection_ = NULL;
    }
    last_output_surface_id_ = output_surface_id;
  }
  if (frame_size.IsEmpty()) {
    DCHECK_EQ(0u, frame_data->resource_list.size());
    EvictDelegatedFrame();
  } else {
    if (!resource_collection_) {
      resource_collection_ = new cc::DelegatedFrameResourceCollection;
      resource_collection_->SetClient(this);
    }
    if (!frame_provider_.get() || frame_size != frame_provider_->frame_size() ||
        frame_size_in_dip != current_frame_size_) {
      frame_provider_ = new cc::DelegatedFrameProvider(
          resource_collection_.get(), frame_data.Pass());
      window_->layer()->SetShowDelegatedContent(frame_provider_.get(),
                                                frame_size_in_dip);
    } else {
      frame_provider_->SetFrameData(frame_data.Pass());
    }
  }
  released_front_lock_ = NULL;
  current_frame_size_ = frame_size_in_dip;
  CheckResizeLock();

  if (paint_observer_)
    paint_observer_->OnUpdateCompositorContent();
  window_->SchedulePaintInRect(damage_rect_in_dip);

  pending_delegated_ack_count_++;

  ui::Compositor* compositor = GetCompositor();
  if (!compositor) {
    SendDelegatedFrameAck(output_surface_id);
  } else {
    for (size_t i = 0; i < latency_info.size(); i++)
      compositor->SetLatencyInfo(latency_info[i]);
    AddOnCommitCallbackAndDisableLocks(
        base::Bind(&RenderWidgetHostViewAura::SendDelegatedFrameAck,
                   AsWeakPtr(),
                   output_surface_id));
  }
  DidReceiveFrameFromRenderer();
  if (frame_provider_.get())
    delegated_frame_evictor_->SwappedFrame(!host_->is_hidden());
}
