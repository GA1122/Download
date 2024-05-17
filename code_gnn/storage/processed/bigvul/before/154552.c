error::Error GLES2DecoderPassthroughImpl::DoActiveTexture(GLenum texture) {
  CheckErrorCallbackState();
  api()->glActiveTextureFn(texture);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  active_texture_unit_ = static_cast<size_t>(texture) - GL_TEXTURE0;
  DCHECK(active_texture_unit_ < kMaxTextureUnits);

  return error::kNoError;
}
