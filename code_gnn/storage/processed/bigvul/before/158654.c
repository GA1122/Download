bool GLES2DecoderImpl::ClearCompressedTextureLevel(Texture* texture,
                                                   unsigned target,
                                                   int level,
                                                   unsigned format,
                                                   int width,
                                                   int height) {
  DCHECK(target != GL_TEXTURE_3D && target != GL_TEXTURE_2D_ARRAY);
  DCHECK(feature_info_->IsWebGL2OrES3Context());

  GLsizei bytes_required = 0;
  if (!GetCompressedTexSizeInBytes("ClearCompressedTextureLevel", width, height,
                                   1, format, &bytes_required,
                                   state_.GetErrorState())) {
    return false;
  }

  TRACE_EVENT1("gpu", "GLES2DecoderImpl::ClearCompressedTextureLevel",
               "bytes_required", bytes_required);

  api()->glBindBufferFn(GL_PIXEL_UNPACK_BUFFER, 0);
  {
    std::unique_ptr<char[]> zero(new char[bytes_required]);
    memset(zero.get(), 0, bytes_required);
    api()->glBindTextureFn(texture->target(), texture->service_id());
    api()->glCompressedTexSubImage2DFn(target, level, 0, 0, width, height,
                                       format, bytes_required, zero.get());
  }
  TextureRef* bound_texture =
      texture_manager()->GetTextureInfoForTarget(&state_, texture->target());
  api()->glBindTextureFn(texture->target(),
                         bound_texture ? bound_texture->service_id() : 0);
  Buffer* bound_buffer = buffer_manager()->GetBufferInfoForTarget(
      &state_, GL_PIXEL_UNPACK_BUFFER);
  if (bound_buffer) {
    api()->glBindBufferFn(GL_PIXEL_UNPACK_BUFFER, bound_buffer->service_id());
  }
  return true;
}
