MojoResult Core::UnmapBuffer(void* buffer) {
  RequestContext request_context;
  std::unique_ptr<PlatformSharedBufferMapping> mapping;
  MojoResult result;
  {
    base::AutoLock lock(mapping_table_lock_);
    result = mapping_table_.RemoveMapping(buffer, &mapping);
  }
  return result;
}
