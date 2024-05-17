DrawingBuffer::ColorBuffer::~ColorBuffer() {
  gpu::gles2::GLES2Interface* gl = drawing_buffer->gl_;
  if (receive_sync_token.HasData())
    gl->WaitSyncTokenCHROMIUM(receive_sync_token.GetConstData());
  if (image_id) {
    gl->BindTexture(parameters.target, texture_id);
    gl->ReleaseTexImage2DCHROMIUM(parameters.target, image_id);
    if (rgb_workaround_texture_id) {
      gl->BindTexture(parameters.target, rgb_workaround_texture_id);
      gl->ReleaseTexImage2DCHROMIUM(parameters.target, image_id);
    }
    gl->DestroyImageCHROMIUM(image_id);
    switch (parameters.target) {
      case GL_TEXTURE_2D:
        if (drawing_buffer->client_)
          drawing_buffer->client_->DrawingBufferClientRestoreTexture2DBinding();
        break;
      case GC3D_TEXTURE_RECTANGLE_ARB:
        break;
      default:
        NOTREACHED();
        break;
    }
    gpu_memory_buffer.reset();
  }
  gl->DeleteTextures(1, &texture_id);
  if (rgb_workaround_texture_id) {
    gl->DeleteTextures(1, &rgb_workaround_texture_id);
  }
}
