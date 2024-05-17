void GpuProcessHost::StoreShaderToDisk(int32_t client_id,
                                       const std::string& key,
                                       const std::string& shader) {
  TRACE_EVENT0("gpu", "GpuProcessHost::StoreShaderToDisk");
  ClientIdToShaderCacheMap::iterator iter =
      client_id_to_shader_cache_.find(client_id);
  if (iter == client_id_to_shader_cache_.end())
    return;
  iter->second->Cache(GetShaderPrefixKey() + ":" + key, shader);
}
