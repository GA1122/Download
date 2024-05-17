error::Error GLES2DecoderPassthroughImpl::DoScheduleDCLayerCHROMIUM(
    GLuint y_texture_id,
    GLuint uv_texture_id,
    GLint z_order,
    GLint content_x,
    GLint content_y,
    GLint content_width,
    GLint content_height,
    GLint quad_x,
    GLint quad_y,
    GLint quad_width,
    GLint quad_height,
    GLfloat transform_c1r1,
    GLfloat transform_c2r1,
    GLfloat transform_c1r2,
    GLfloat transform_c2r2,
    GLfloat transform_tx,
    GLfloat transform_ty,
    GLboolean is_clipped,
    GLint clip_x,
    GLint clip_y,
    GLint clip_width,
    GLint clip_height,
    GLuint protected_video_type) {
  if (protected_video_type >
      static_cast<GLuint>(ui::ProtectedVideoType::kMaxValue)) {
    InsertError(GL_INVALID_VALUE, "invalid protected video type");
    return error::kNoError;
  }

  GLuint texture_ids[] = {y_texture_id, uv_texture_id};
  scoped_refptr<gl::GLImage> images[2];
  size_t i = 0;
  for (GLuint texture_id : texture_ids) {
    if (!texture_id) {
      InsertError(GL_INVALID_VALUE, "invalid texture");
      return error::kNoError;
    }

    scoped_refptr<TexturePassthrough> passthrough_texture;
    if (!resources_->texture_object_map.GetServiceID(texture_id,
                                                     &passthrough_texture)) {
      InsertError(GL_INVALID_VALUE, "unknown texture");
      return error::kNoError;
    }
    DCHECK(passthrough_texture);
    gl::GLImage* image =
        passthrough_texture->GetLevelImage(passthrough_texture->target(), 0);
    if (!image) {
      InsertError(GL_INVALID_VALUE, "unsupported texture format");
      return error::kNoError;
    }
    images[i++] = scoped_refptr<gl::GLImage>(image);
  }

  ui::DCRendererLayerParams params;
  params.y_image = std::move(images[0]);
  params.uv_image = std::move(images[1]);
  params.z_order = z_order;
  params.content_rect =
      gfx::Rect(content_x, content_y, content_width, content_height);
  params.quad_rect = gfx::Rect(quad_x, quad_y, quad_width, quad_height);
  params.transform =
      gfx::Transform(transform_c1r1, transform_c2r1, transform_c1r2,
                     transform_c2r2, transform_tx, transform_ty);
  params.is_clipped = is_clipped;
  params.clip_rect = gfx::Rect(clip_x, clip_y, clip_width, clip_height);
  params.protected_video_type =
      static_cast<ui::ProtectedVideoType>(protected_video_type);

  if (!surface_->ScheduleDCLayer(params))
    InsertError(GL_INVALID_OPERATION, "failed to schedule DCLayer");

  return error::kNoError;
}
