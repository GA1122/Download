error::Error GLES2DecoderImpl::HandleScheduleCALayerCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::ScheduleCALayerCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::ScheduleCALayerCHROMIUM*>(
          cmd_data);
  GLuint filter = c.filter;
  if (filter != GL_NEAREST && filter != GL_LINEAR) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glScheduleCALayerCHROMIUM",
                       "invalid filter");
    return error::kNoError;
  }

  if (!ca_layer_shared_state_) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, "glScheduleCALayerCHROMIUM",
        "glScheduleCALayerSharedStateCHROMIUM has not been called");
    return error::kNoError;
  }

  gl::GLImage* image = nullptr;
  GLuint contents_texture_id = c.contents_texture_id;
  if (contents_texture_id) {
    TextureRef* ref = texture_manager()->GetTexture(contents_texture_id);
    if (!ref) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glScheduleCALayerCHROMIUM",
                         "unknown texture");
      return error::kNoError;
    }
    Texture::ImageState image_state;
    image = ref->texture()->GetLevelImage(ref->texture()->target(), 0,
                                          &image_state);
    if (!image) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glScheduleCALayerCHROMIUM",
                         "unsupported texture format");
      return error::kNoError;
    }
  }

  const GLfloat* mem = GetSharedMemoryAs<const GLfloat*>(c.shm_id, c.shm_offset,
                                                         8 * sizeof(GLfloat));
  if (!mem) {
    return error::kOutOfBounds;
  }
  gfx::RectF contents_rect(mem[0], mem[1], mem[2], mem[3]);
  gfx::RectF bounds_rect(mem[4], mem[5], mem[6], mem[7]);

  ui::CARendererLayerParams params = ui::CARendererLayerParams(
      ca_layer_shared_state_->is_clipped, ca_layer_shared_state_->clip_rect,
      ca_layer_shared_state_->sorting_context_id,
      ca_layer_shared_state_->transform, image, contents_rect,
      gfx::ToEnclosingRect(bounds_rect), c.background_color, c.edge_aa_mask,
      ca_layer_shared_state_->opacity, filter);
  if (!surface_->ScheduleCALayer(params)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glScheduleCALayerCHROMIUM",
                       "failed to schedule CALayer");
  }
  return error::kNoError;
}
