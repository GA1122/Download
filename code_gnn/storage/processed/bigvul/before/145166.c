void GpuProcessHost::LoadedShader(int32_t client_id,
                                  const std::string& key,
                                  const std::string& data) {
  std::string prefix = GetShaderPrefixKey();
  bool prefix_ok = !key.compare(0, prefix.length(), prefix);
  UMA_HISTOGRAM_BOOLEAN("GPU.ShaderLoadPrefixOK", prefix_ok);
  if (prefix_ok) {
    std::string key_no_prefix = key.substr(prefix.length() + 1);
    gpu_service_ptr_->LoadedShader(client_id, key_no_prefix, data);
  }
}
