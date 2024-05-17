void GLES2DecoderImpl::DeleteSyncHelper(GLuint sync) {
  GLsync service_id = 0;
  if (group_->GetSyncServiceId(sync, &service_id)) {
    api()->glDeleteSyncFn(service_id);
    group_->RemoveSyncId(sync);
  } else if (sync != 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glDeleteSync", "unknown sync");
  }
}
