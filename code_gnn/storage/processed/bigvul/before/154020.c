void GLES2DecoderImpl::DoGetSynciv(GLuint sync_id,
                                   GLenum pname,
                                   GLsizei num_values,
                                   GLsizei* length,
                                   GLint* values) {
  GLsync service_sync = 0;
  if (!group_->GetSyncServiceId(sync_id, &service_sync)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glGetSynciv", "invalid sync id");
    return;
  }
  api()->glGetSyncivFn(service_sync, pname, num_values, nullptr, values);
}
