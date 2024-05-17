int GetChannelID(gpu::CommandBufferId command_buffer_id) {
  return static_cast<int>(command_buffer_id.GetUnsafeValue() >> 32);
}
