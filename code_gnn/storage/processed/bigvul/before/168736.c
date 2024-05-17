void RenderWidgetHostImpl::DidAllocateLocalSurfaceIdForAutoResize(
    uint64_t sequence_number) {
  if (!view_ || !sequence_number ||
      last_auto_resize_request_number_ != sequence_number) {
    return;
  }

  DCHECK(!view_->IsLocalSurfaceIdAllocationSuppressed());

  viz::LocalSurfaceId local_surface_id(view_->GetLocalSurfaceId());
  if (local_surface_id.is_valid()) {
    ScreenInfo screen_info;
    view_->GetScreenInfo(&screen_info);
    Send(new ViewMsg_SetLocalSurfaceIdForAutoResize(
        routing_id_, sequence_number, min_size_for_auto_resize_,
        max_size_for_auto_resize_, screen_info, current_content_source_id_,
        local_surface_id));
  }
}
