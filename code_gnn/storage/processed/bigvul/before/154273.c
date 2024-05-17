error::Error GLES2DecoderImpl::HandleScheduleOverlayPlaneCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::ScheduleOverlayPlaneCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::ScheduleOverlayPlaneCHROMIUM*>(
          cmd_data);
  TextureRef* ref = texture_manager()->GetTexture(c.overlay_texture_id);
  if (!ref) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE,
                       "glScheduleOverlayPlaneCHROMIUM",
                       "unknown texture");
    return error::kNoError;
  }
  Texture::ImageState image_state;
  gl::GLImage* image =
      ref->texture()->GetLevelImage(ref->texture()->target(), 0, &image_state);
  if (!image) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE,
                       "glScheduleOverlayPlaneCHROMIUM",
                       "unsupported texture format");
    return error::kNoError;
  }
  gfx::OverlayTransform transform = GetGFXOverlayTransform(c.plane_transform);
  if (transform == gfx::OVERLAY_TRANSFORM_INVALID) {
    LOCAL_SET_GL_ERROR(GL_INVALID_ENUM,
                       "glScheduleOverlayPlaneCHROMIUM",
                       "invalid transform enum");
    return error::kNoError;
  }
  GLuint gpu_fence_id = static_cast<GLuint>(c.gpu_fence_id);
  std::unique_ptr<gfx::GpuFence> gpu_fence;
  if (gpu_fence_id > 0) {
    gpu_fence = GetGpuFenceManager()->GetGpuFence(gpu_fence_id);
    if (!gpu_fence) {
      LOCAL_SET_GL_ERROR(GL_INVALID_ENUM, "glScheduleOverlayPlaneCHROMIUM",
                         "unknown fence");
      return error::kNoError;
    }
  }
  if (!surface_->ScheduleOverlayPlane(
          c.plane_z_order, transform, image,
          gfx::Rect(c.bounds_x, c.bounds_y, c.bounds_width, c.bounds_height),
          gfx::RectF(c.uv_x, c.uv_y, c.uv_width, c.uv_height), c.enable_blend,
          std::move(gpu_fence))) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
                       "glScheduleOverlayPlaneCHROMIUM",
                       "failed to schedule overlay");
  }
  return error::kNoError;
}
