void RenderWidgetHostImpl::SubmitCompositorFrame(
    const viz::LocalSurfaceId& local_surface_id,
    viz::CompositorFrame frame,
    viz::mojom::HitTestRegionListPtr hit_test_region_list,
    uint64_t submit_time) {
  TRACE_EVENT_FLOW_END0(TRACE_DISABLED_BY_DEFAULT("cc.debug.ipc"),
                        "SubmitCompositorFrame", local_surface_id.hash());

  if (frame.HasCopyOutputRequests()) {
    bad_message::ReceivedBadMessage(GetProcess(),
                                    bad_message::RWH_COPY_REQUEST_ATTEMPT);
    return;
  }

  bool tracing_enabled;
  TRACE_EVENT_CATEGORY_GROUP_ENABLED(TRACE_DISABLED_BY_DEFAULT("cc.debug.ipc"),
                                     &tracing_enabled);
  if (tracing_enabled) {
    base::TimeDelta elapsed = base::TimeTicks::Now().since_origin() -
                              base::TimeDelta::FromMicroseconds(submit_time);
    TRACE_EVENT_INSTANT1(TRACE_DISABLED_BY_DEFAULT("cc.debug.ipc"),
                         "SubmitCompositorFrame::TimeElapsed",
                         TRACE_EVENT_SCOPE_THREAD,
                         "elapsed time:", elapsed.InMicroseconds());
  }
  auto new_surface_properties =
      RenderWidgetSurfaceProperties::FromCompositorFrame(frame);

  if (local_surface_id == last_local_surface_id_ &&
      SurfacePropertiesMismatch(new_surface_properties,
                                last_surface_properties_)) {
    std::string error = base::StringPrintf(
        "[OOPIF? %d] %s\n", view_ && view_->IsRenderWidgetHostViewChildFrame(),
        new_surface_properties.ToDiffString(last_surface_properties_).c_str());
    LOG(ERROR) << "Surface invariants violation: " << error;

    static int invariants_violation_count = 0;
    ++invariants_violation_count;
    UMA_HISTOGRAM_COUNTS_1000("Compositing.SurfaceInvariantsViolations",
                              invariants_violation_count);

    if (features::IsSurfaceInvariantsViolationLoggingEnabled()) {
      static auto* crash_key = base::debug::AllocateCrashKeyString(
          "surface-invariants-violation", base::debug::CrashKeySize::Size256);
      base::debug::ScopedCrashKeyString key_value(crash_key, error);
      base::debug::DumpWithoutCrashing();
    }

    if (view_) {
      frame.metadata.begin_frame_ack.has_damage = false;
      view_->OnDidNotProduceFrame(frame.metadata.begin_frame_ack);
    }
    std::vector<viz::ReturnedResource> resources =
        viz::TransferableResource::ReturnResources(frame.resource_list);
    renderer_compositor_frame_sink_->DidReceiveCompositorFrameAck(resources);

    return;
  }

  uint32_t max_sequence_number = 0;
  for (const auto& resource : frame.resource_list) {
    max_sequence_number =
        std::max(max_sequence_number, resource.shared_bitmap_sequence_number);
  }

  uint32_t last_registered_sequence_number =
      GetProcess()->GetSharedBitmapAllocationNotifier()->last_sequence_number();
  if (max_sequence_number > last_registered_sequence_number) {
    saved_frame_.frame = std::move(frame);
    saved_frame_.local_surface_id = local_surface_id;
    saved_frame_.max_shared_bitmap_sequence_number = max_sequence_number;
    saved_frame_.hit_test_region_list = std::move(hit_test_region_list);
    TRACE_EVENT_ASYNC_BEGIN2("renderer_host", "PauseCompositorFrameSink", this,
                             "LastRegisteredSequenceNumber",
                             last_registered_sequence_number,
                             "RequiredSequenceNumber", max_sequence_number);
    compositor_frame_sink_binding_.PauseIncomingMethodCallProcessing();
    return;
  }

  last_local_surface_id_ = local_surface_id;
  last_surface_properties_ = new_surface_properties;

  last_received_content_source_id_ = frame.metadata.content_source_id;

  frame.metadata.begin_frame_ack.has_damage = true;

  last_frame_metadata_ = frame.metadata.Clone();

  latency_tracker_.OnSwapCompositorFrame(&frame.metadata.latency_info);

  bool is_mobile_optimized = IsMobileOptimizedFrame(frame.metadata);
  input_router_->NotifySiteIsMobileOptimized(is_mobile_optimized);
  if (touch_emulator_)
    touch_emulator_->SetDoubleTapSupportForPageEnabled(!is_mobile_optimized);

  if (enable_surface_synchronization_) {
    if (view_) {
      view_->SubmitCompositorFrame(local_surface_id, std::move(frame),
                                   std::move(hit_test_region_list));
      view_->DidReceiveRendererFrame();
    } else {
      std::vector<viz::ReturnedResource> resources =
          viz::TransferableResource::ReturnResources(frame.resource_list);
      renderer_compositor_frame_sink_->DidReceiveCompositorFrameAck(resources);
    }
  } else {
    if (view_ &&
        frame.metadata.content_source_id >= current_content_source_id_) {
      view_->SubmitCompositorFrame(local_surface_id, std::move(frame),
                                   std::move(hit_test_region_list));
      view_->DidReceiveRendererFrame();
    } else {
      if (view_) {
        frame.metadata.begin_frame_ack.has_damage = false;
        view_->OnDidNotProduceFrame(frame.metadata.begin_frame_ack);
      }
      std::vector<viz::ReturnedResource> resources =
          viz::TransferableResource::ReturnResources(frame.resource_list);
      renderer_compositor_frame_sink_->DidReceiveCompositorFrameAck(resources);
    }

    if (new_content_rendering_timeout_ &&
        last_received_content_source_id_ >= current_content_source_id_ &&
        new_content_rendering_timeout_->IsRunning()) {
      new_content_rendering_timeout_->Stop();
    }
  }

  if (delegate_)
    delegate_->DidReceiveCompositorFrame();
}
