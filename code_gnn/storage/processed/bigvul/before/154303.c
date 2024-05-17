bool GLES2DecoderImpl::InitializeCopyTexImageBlitter(
    const char* function_name) {
  if (!copy_tex_image_blit_.get()) {
    LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER(function_name);
    copy_tex_image_blit_.reset(
        new CopyTexImageResourceManager(feature_info_.get()));
    copy_tex_image_blit_->Initialize(this);
    if (LOCAL_PEEK_GL_ERROR(function_name) != GL_NO_ERROR)
      return false;
  }
  return true;
}
