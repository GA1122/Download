void GLES2Implementation::InvalidateReadbackBufferShadowDataCHROMIUM(
    GLuint buffer_id) {
  readback_buffer_shadow_tracker_->OnBufferWrite(buffer_id);
}
