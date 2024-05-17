error::Error GLES2DecoderPassthroughImpl::DoDeleteSync(GLuint sync) {
  return DeleteHelper(sync, &resources_->sync_id_map, [this](uintptr_t sync) {
    api()->glDeleteSyncFn(reinterpret_cast<GLsync>(sync));
  });
}
