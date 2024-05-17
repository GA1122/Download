bool RenderWidgetHostImpl::OnSwapCompositorFrame(
    const IPC::Message& message) {
  TRACE_EVENT0("test_fps,benchmark", "OnSwapCompositorFrame");

  ViewHostMsg_SwapCompositorFrame::Param param;
  if (!ViewHostMsg_SwapCompositorFrame::Read(&message, &param))
    return false;
  scoped_ptr<cc::CompositorFrame> frame(new cc::CompositorFrame);
  uint32_t output_surface_id = base::get<0>(param);
  base::get<1>(param).AssignTo(frame.get());
  std::vector<IPC::Message> messages_to_deliver_with_frame;
  messages_to_deliver_with_frame.swap(base::get<2>(param));

  if (!ui::LatencyInfo::Verify(frame->metadata.latency_info,
                               "RenderWidgetHostImpl::OnSwapCompositorFrame")) {
    std::vector<ui::LatencyInfo>().swap(frame->metadata.latency_info);
  }

  latency_tracker_.OnSwapCompositorFrame(&frame->metadata.latency_info);

  bool is_mobile_optimized = IsMobileOptimizedFrame(frame->metadata);
  input_router_->NotifySiteIsMobileOptimized(is_mobile_optimized);
  if (touch_emulator_)
    touch_emulator_->SetDoubleTapSupportForPageEnabled(!is_mobile_optimized);

  if (view_) {
    view_->OnSwapCompositorFrame(output_surface_id, std::move(frame));
    view_->DidReceiveRendererFrame();
  } else {
    cc::CompositorFrameAck ack;
    if (frame->gl_frame_data) {
      ack.gl_frame_data = std::move(frame->gl_frame_data);
      ack.gl_frame_data->sync_token.Clear();
    } else if (frame->delegated_frame_data) {
      cc::TransferableResource::ReturnResources(
          frame->delegated_frame_data->resource_list,
          &ack.resources);
    }
    SendSwapCompositorFrameAck(routing_id_, output_surface_id,
                               process_->GetID(), ack);
  }

  RenderProcessHost* rph = GetProcess();
  for (std::vector<IPC::Message>::const_iterator i =
           messages_to_deliver_with_frame.begin();
       i != messages_to_deliver_with_frame.end();
       ++i) {
    rph->OnMessageReceived(*i);
    if (i->dispatch_error())
      rph->OnBadMessageReceived(*i);
  }
  messages_to_deliver_with_frame.clear();

  return true;
}
