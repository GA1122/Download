void BrowserPluginGuest::UpdateRect(
    RenderViewHost* render_view_host,
    const ViewHostMsg_UpdateRect_Params& params) {
  if (!params.needs_ack)
    return;

  if (((auto_size_enabled_ && InAutoSizeBounds(params.view_size)) ||
      (params.view_size.width() == damage_view_size().width() &&
       params.view_size.height() == damage_view_size().height())) &&
       params.scale_factor == damage_buffer_scale_factor()) {
    TransportDIB* dib = render_view_host->GetProcess()->
        GetTransportDIB(params.bitmap);
    if (dib) {
#if defined(OS_WIN)
      size_t guest_damage_buffer_size = params.bitmap_rect.width() *
                                        params.bitmap_rect.height() * 4;
      size_t embedder_damage_buffer_size = damage_buffer_size_;
#else
      size_t guest_damage_buffer_size = dib->size();
      size_t embedder_damage_buffer_size = damage_buffer_->size();
#endif
      void* guest_memory = dib->memory();
      void* embedder_memory = damage_buffer_->memory();
      size_t size = std::min(guest_damage_buffer_size,
                             embedder_damage_buffer_size);
      memcpy(embedder_memory, guest_memory, size);
    }
  }
  BrowserPluginMsg_UpdateRect_Params relay_params;
#if defined(OS_MACOSX)
  relay_params.damage_buffer_identifier = damage_buffer_->id();
#elif defined(OS_WIN)
  relay_params.damage_buffer_identifier = remote_damage_buffer_handle_;
#else
  relay_params.damage_buffer_identifier = damage_buffer_->handle();
#endif
  relay_params.bitmap_rect = params.bitmap_rect;
  relay_params.scroll_delta = params.scroll_delta;
  relay_params.scroll_rect = params.scroll_rect;
  relay_params.copy_rects = params.copy_rects;
  relay_params.view_size = params.view_size;
  relay_params.scale_factor = params.scale_factor;
  relay_params.is_resize_ack = ViewHostMsg_UpdateRect_Flags::is_resize_ack(
      params.flags);

  int message_id = pending_update_counter_++;
  pending_updates_.AddWithID(render_view_host, message_id);

  SendMessageToEmbedder(new BrowserPluginMsg_UpdateRect(embedder_routing_id(),
                                                        instance_id(),
                                                        message_id,
                                                        relay_params));
}
