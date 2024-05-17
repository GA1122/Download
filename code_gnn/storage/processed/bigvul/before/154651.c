error::Error GLES2DecoderPassthroughImpl::DoFenceSync(GLenum condition,
                                                      GLbitfield flags,
                                                      GLuint client_id) {
  if (resources_->sync_id_map.HasClientID(client_id)) {
    return error::kInvalidArguments;
  }

  CheckErrorCallbackState();
  GLsync service_id = api()->glFenceSyncFn(condition, flags);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  resources_->sync_id_map.SetIDMapping(client_id,
                                       reinterpret_cast<uintptr_t>(service_id));

  return error::kNoError;
}
