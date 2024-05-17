error::Error GLES2DecoderImpl::HandleSetColorSpaceMetadataCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::SetColorSpaceMetadataCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::SetColorSpaceMetadataCHROMIUM*>(
          cmd_data);

  GLuint texture_id = c.texture_id;
  GLsizei color_space_size = c.color_space_size;
  const char* data = static_cast<const char*>(
      GetAddressAndCheckSize(c.shm_id, c.shm_offset, color_space_size));
  if (!data)
    return error::kOutOfBounds;

  std::vector<char> color_space_data(data, data + color_space_size);
  base::Pickle color_space_pickle(color_space_data.data(), color_space_size);
  base::PickleIterator iterator(color_space_pickle);
  gfx::ColorSpace color_space;
  if (!IPC::ParamTraits<gfx::ColorSpace>::Read(&color_space_pickle, &iterator,
                                               &color_space))
    return error::kOutOfBounds;

  TextureRef* ref = texture_manager()->GetTexture(texture_id);
  if (!ref) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glSetColorSpaceMetadataCHROMIUM",
                       "unknown texture");
    return error::kNoError;
  }

  scoped_refptr<gl::GLImage> image =
      ref->texture()->GetLevelImage(ref->texture()->target(), 0);
  if (!image) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glSetColorSpaceMetadataCHROMIUM",
                       "no image associated with texture");
    return error::kNoError;
  }

  image->SetColorSpace(color_space);
  return error::kNoError;
}
