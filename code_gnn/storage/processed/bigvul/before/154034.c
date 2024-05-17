bool GLES2DecoderImpl::DoIsSync(GLuint client_id) {
  GLsync service_sync = 0;
  return group_->GetSyncServiceId(client_id, &service_sync);
}
