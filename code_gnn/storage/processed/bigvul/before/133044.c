void RenderWidgetHostViewAura::SwapSoftwareFrame(
    uint32 output_surface_id,
    scoped_ptr<cc::SoftwareFrameData> frame_data,
    float frame_device_scale_factor,
    const std::vector<ui::LatencyInfo>& latency_info) {
  const gfx::Size& frame_size = frame_data->size;
  const gfx::Rect& damage_rect = frame_data->damage_rect;
  gfx::Size frame_size_in_dip =
      ConvertSizeToDIP(frame_device_scale_factor, frame_size);
  if (ShouldSkipFrame(frame_size_in_dip)) {
    ReleaseSoftwareFrame(output_surface_id, frame_data->id);
    SendSoftwareFrameAck(output_surface_id);
    return;
  }

  if (!software_frame_manager_->SwapToNewFrame(
          output_surface_id,
          frame_data.get(),
          frame_device_scale_factor,
          host_->GetProcess()->GetHandle())) {
    ReleaseSoftwareFrame(output_surface_id, frame_data->id);
    SendSoftwareFrameAck(output_surface_id);
    return;
  }

  if (last_swapped_surface_size_ != frame_size) {
    DLOG_IF(ERROR, damage_rect != gfx::Rect(frame_size))
        << "Expected full damage rect";
  }
  last_swapped_surface_size_ = frame_size;
  last_swapped_surface_scale_factor_ = frame_device_scale_factor;

  cc::TextureMailbox mailbox;
  scoped_ptr<cc::SingleReleaseCallback> callback;
  software_frame_manager_->GetCurrentFrameMailbox(&mailbox, &callback);
  DCHECK(mailbox.IsSharedMemory());
  current_frame_size_ = frame_size_in_dip;

  released_front_lock_ = NULL;
  CheckResizeLock();
  window_->layer()->SetTextureMailbox(mailbox,
                                      callback.Pass(),
                                      frame_device_scale_factor);
  window_->SchedulePaintInRect(
      ConvertRectToDIP(frame_device_scale_factor, damage_rect));

  ui::Compositor* compositor = GetCompositor();
  if (compositor) {
    for (size_t i = 0; i < latency_info.size(); i++)
      compositor->SetLatencyInfo(latency_info[i]);
    AddOnCommitCallbackAndDisableLocks(
        base::Bind(&RenderWidgetHostViewAura::SendSoftwareFrameAck,
                   AsWeakPtr(),
                   output_surface_id));
  }
  if (paint_observer_)
    paint_observer_->OnUpdateCompositorContent();
  DidReceiveFrameFromRenderer();

  software_frame_manager_->SwapToNewFrameComplete(!host_->is_hidden());
}
