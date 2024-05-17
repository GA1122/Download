void GpuProcessHost::CreateChannelCache(int32_t client_id) {
  TRACE_EVENT0("gpu", "GpuProcessHost::CreateChannelCache");

  scoped_refptr<gpu::ShaderDiskCache> cache =
      GetShaderCacheFactorySingleton()->Get(client_id);
  if (!cache.get())
    return;

  cache->set_shader_loaded_callback(base::Bind(&GpuProcessHost::LoadedShader,
                                               weak_ptr_factory_.GetWeakPtr(),
                                               client_id));

  client_id_to_shader_cache_[client_id] = cache;
}
