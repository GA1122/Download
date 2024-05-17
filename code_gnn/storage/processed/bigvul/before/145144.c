void GpuProcessHost::DidDestroyChannel(int32_t client_id) {
  TRACE_EVENT0("gpu", "GpuProcessHost::DidDestroyChannel");
  client_id_to_shader_cache_.erase(client_id);
}
