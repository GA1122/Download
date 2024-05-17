error::Error GLES2DecoderImpl::HandleCompressedTexSubImage3DBucket(
    uint32_t immediate_data_size, const volatile void* cmd_data) {
  if (!feature_info_->IsWebGL2OrES3Context())
    return error::kUnknownCommand;
  const volatile gles2::cmds::CompressedTexSubImage3DBucket& c =
      *static_cast<const volatile gles2::cmds::CompressedTexSubImage3DBucket*>(
          cmd_data);
  GLenum target = static_cast<GLenum>(c.target);
  GLint level = static_cast<GLint>(c.level);
  GLint xoffset = static_cast<GLint>(c.xoffset);
  GLint yoffset = static_cast<GLint>(c.yoffset);
  GLint zoffset = static_cast<GLint>(c.zoffset);
  GLsizei width = static_cast<GLsizei>(c.width);
  GLsizei height = static_cast<GLsizei>(c.height);
  GLsizei depth = static_cast<GLsizei>(c.depth);
  GLenum format = static_cast<GLenum>(c.format);
  GLuint bucket_id = static_cast<GLuint>(c.bucket_id);

  if (state_.bound_pixel_unpack_buffer.get()) {
    return error::kInvalidArguments;
  }
  Bucket* bucket = GetBucket(bucket_id);
  if (!bucket)
    return error::kInvalidArguments;
  uint32_t image_size = bucket->size();
  const void* data = bucket->GetData(0, image_size);
  DCHECK(data || !image_size);
  return DoCompressedTexSubImage(target, level, xoffset, yoffset, zoffset,
                                 width, height, depth, format, image_size,
                                 data, ContextState::k3D);
}
