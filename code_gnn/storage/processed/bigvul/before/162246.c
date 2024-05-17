gpu::CommandBufferId CommandBufferProxyID(int channel_id, int32_t route_id) {
  return gpu::CommandBufferId::FromUnsafeValue(
      (static_cast<uint64_t>(channel_id) << 32) | route_id);
}
