void GLES2Implementation::DestroyImageCHROMIUMHelper(GLuint image_id) {
  helper_->CommandBufferHelper::Flush();
  gpu_control_->DestroyImage(image_id);
}
