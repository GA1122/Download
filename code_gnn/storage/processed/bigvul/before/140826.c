error::Error GLES2DecoderImpl::HandleTexSubImage3D(uint32 immediate_data_size,
                                                   const void* cmd_data) {
  if (!unsafe_es3_apis_enabled())
    return error::kUnknownCommand;

  const gles2::cmds::TexSubImage3D& c =
      *static_cast<const gles2::cmds::TexSubImage3D*>(cmd_data);
  TRACE_EVENT2("gpu", "GLES2DecoderImpl::HandleTexSubImage3D",
      "widthXheight", c.width * c.height, "depth", c.depth);
  GLenum target = static_cast<GLenum>(c.target);
  GLint level = static_cast<GLint>(c.level);
  GLint xoffset = static_cast<GLint>(c.xoffset);
  GLint yoffset = static_cast<GLint>(c.yoffset);
  GLint zoffset = static_cast<GLint>(c.zoffset);
  GLsizei width = static_cast<GLsizei>(c.width);
  GLsizei height = static_cast<GLsizei>(c.height);
  GLsizei depth = static_cast<GLsizei>(c.depth);
  GLenum format = static_cast<GLenum>(c.format);
  GLenum type = static_cast<GLenum>(c.type);
  uint32 data_size;
  if (!GLES2Util::ComputeImageDataSizes(
      width, height, depth, format, type, state_.unpack_alignment, &data_size,
      NULL, NULL)) {
    return error::kOutOfBounds;
  }
  const void* pixels = GetSharedMemoryAs<const void*>(
      c.pixels_shm_id, c.pixels_shm_offset, data_size);
  glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height,
                  depth, format, type, pixels);
  return error::kNoError;
}
