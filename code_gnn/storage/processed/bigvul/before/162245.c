bool CommandBufferProxyImpl::CanWaitUnverifiedSyncToken(
    const gpu::SyncToken& sync_token) {
  int sync_token_channel_id = GetChannelID(sync_token.command_buffer_id());
  if (sync_token.namespace_id() != gpu::CommandBufferNamespace::GPU_IO ||
      sync_token_channel_id != channel_id_) {
    return false;
  }
  return true;
}
