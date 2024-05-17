bool GLES2DecoderImpl::InitializeCopyTextureCHROMIUM(
    const char* function_name) {
  if (!copy_texture_chromium_.get()) {
    LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER(function_name);
    copy_texture_chromium_.reset(CopyTextureCHROMIUMResourceManager::Create());
    copy_texture_chromium_->Initialize(this, features());
    if (LOCAL_PEEK_GL_ERROR(function_name) != GL_NO_ERROR)
      return false;

    if (CopyTexImageResourceManager::CopyTexImageRequiresBlit(
            feature_info_.get(), GL_LUMINANCE)) {
      if (!InitializeCopyTexImageBlitter(function_name))
        return false;
    }
  }
  return true;
}
