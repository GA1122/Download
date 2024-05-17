error::Error GLES2DecoderImpl::HandleScheduleDCLayerCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::ScheduleDCLayerCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::ScheduleDCLayerCHROMIUM*>(
          cmd_data);
  GLuint filter = c.filter;
  if (filter != GL_NEAREST && filter != GL_LINEAR) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glScheduleDCLayerCHROMIUM",
                       "invalid filter");
    return error::kNoError;
  }

  if (!dc_layer_shared_state_) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, "glScheduleDCLayerCHROMIUM",
        "glScheduleDCLayerSharedStateCHROMIUM has not been called");
    return error::kNoError;
  }

  GLsizei num_textures = c.num_textures;
  if (num_textures < 0 || num_textures > 4) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glScheduleDCLayerCHROMIUM",
                       "number of textures greater than maximum of 4");
    return error::kNoError;
  }

  size_t textures_size = num_textures * sizeof(GLuint);

  base::CheckedNumeric<uint32_t> data_size = textures_size;
  const uint32_t kRectDataSize = 8 * sizeof(GLfloat);
  data_size += kRectDataSize;
  if (!data_size.IsValid())
    return error::kOutOfBounds;
  const void* data =
      GetAddressAndCheckSize(c.shm_id, c.shm_offset, data_size.ValueOrDie());
  if (!data) {
    return error::kOutOfBounds;
  }
  const GLfloat* mem = reinterpret_cast<const GLfloat*>(data);

  gfx::RectF contents_rect(mem[0], mem[1], mem[2], mem[3]);
  gfx::RectF bounds_rect(mem[4], mem[5], mem[6], mem[7]);

  const volatile GLuint* texture_ids = reinterpret_cast<const volatile GLuint*>(
      static_cast<const volatile char*>(data) + kRectDataSize);

  std::vector<scoped_refptr<gl::GLImage>> images;
  for (int i = 0; i < num_textures; ++i) {
    GLuint contents_texture_id = texture_ids[i];
    scoped_refptr<gl::GLImage> image;
    if (contents_texture_id) {
      TextureRef* ref = texture_manager()->GetTexture(contents_texture_id);
      if (!ref) {
        LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glScheduleDCLayerCHROMIUM",
                           "unknown texture");
        return error::kNoError;
      }
      Texture::ImageState image_state;
      image = ref->texture()->GetLevelImage(ref->texture()->target(), 0,
                                            &image_state);
      if (!image) {
        LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glScheduleDCLayerCHROMIUM",
                           "unsupported texture format");
        return error::kNoError;
      }
    }
    images.push_back(image);
  }

  ui::DCRendererLayerParams params = ui::DCRendererLayerParams(
      dc_layer_shared_state_->is_clipped, dc_layer_shared_state_->clip_rect,
      dc_layer_shared_state_->z_order, dc_layer_shared_state_->transform,
      images, contents_rect, gfx::ToEnclosingRect(bounds_rect),
      c.background_color, c.edge_aa_mask, dc_layer_shared_state_->opacity,
      filter, c.is_protected_video);
  if (!surface_->ScheduleDCLayer(params)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glScheduleDCLayerCHROMIUM",
                       "failed to schedule DCLayer");
  }
  return error::kNoError;
}
