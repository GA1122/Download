bool DataPipeConsumerDispatcher::EndSerialize(
    void* destination,
    ports::PortName* ports,
    PlatformHandle* platform_handles) {
  SerializedState* state = static_cast<SerializedState*>(destination);
  memcpy(&state->options, &options_, sizeof(MojoCreateDataPipeOptions));
  memset(state->padding, 0, sizeof(state->padding));

  base::AutoLock lock(lock_);
  DCHECK(in_transit_);
  state->pipe_id = pipe_id_;
  state->read_offset = read_offset_;
  state->bytes_available = bytes_available_;
  state->flags = peer_closed_ ? kFlagPeerClosed : 0;

  auto region_handle =
      base::UnsafeSharedMemoryRegion::TakeHandleForSerialization(
          std::move(shared_ring_buffer_));
  const base::UnguessableToken& guid = region_handle.GetGUID();
  state->buffer_guid_high = guid.GetHighForSerialization();
  state->buffer_guid_low = guid.GetLowForSerialization();

  ports[0] = control_port_.name();

  PlatformHandle handle;
  PlatformHandle ignored_handle;
  ExtractPlatformHandlesFromSharedMemoryRegionHandle(
      region_handle.PassPlatformHandle(), &handle, &ignored_handle);
  if (!handle.is_valid() || ignored_handle.is_valid())
    return false;

  platform_handles[0] = std::move(handle);
  return true;
}
