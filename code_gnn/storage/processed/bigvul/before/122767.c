void BrowserPluginGuest::SetDamageBuffer(
    TransportDIB* damage_buffer,
#if defined(OS_WIN)
    int damage_buffer_size,
    TransportDIB::Handle remote_handle,
#endif
    const gfx::Size& damage_view_size,
    float scale_factor) {
  DCHECK(*static_cast<unsigned int*>(damage_buffer->memory()) == 0xdeadbeef);
  damage_buffer_.reset(damage_buffer);
#if defined(OS_WIN)
  damage_buffer_size_ = damage_buffer_size;
  remote_damage_buffer_handle_ = remote_handle;
#endif
  damage_view_size_ = damage_view_size;
  damage_buffer_scale_factor_ = scale_factor;
}
