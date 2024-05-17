error::Error GLES2DecoderPassthroughImpl::DoTexStorage3D(GLenum target,
                                                         GLsizei levels,
                                                         GLenum internalFormat,
                                                         GLsizei width,
                                                         GLsizei height,
                                                         GLsizei depth) {
  CheckErrorCallbackState();
  api()->glTexStorage3DFn(target, levels, internalFormat, width, height, depth);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  UpdateTextureSizeFromTarget(target);
  return error::kNoError;
}
