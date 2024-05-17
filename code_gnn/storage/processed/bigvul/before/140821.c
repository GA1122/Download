error::Error GLES2DecoderImpl::HandleScheduleOverlayPlaneCHROMIUM(
    uint32 immediate_data_size,
    const void* cmd_data) {
  const gles2::cmds::ScheduleOverlayPlaneCHROMIUM& c =
      *static_cast<const gles2::cmds::ScheduleOverlayPlaneCHROMIUM*>(cmd_data);
  TextureRef* ref = texture_manager()->GetTexture(c.overlay_texture_id);
  if (!ref) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE,
                       "glScheduleOverlayPlaneCHROMIUM",
                       "unknown texture");
    return error::kNoError;
  }
  gfx::GLImage* image =
      ref->texture()->GetLevelImage(ref->texture()->target(), 0);
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
  if (!surface_->ScheduleOverlayPlane(
          c.plane_z_order,
          transform,
          image,
          gfx::Rect(c.bounds_x, c.bounds_y, c.bounds_width, c.bounds_height),
          gfx::RectF(c.uv_x, c.uv_y, c.uv_width, c.uv_height))) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
                       "glScheduleOverlayPlaneCHROMIUM",
                       "failed to schedule overlay");
  }
  return error::kNoError;
}
