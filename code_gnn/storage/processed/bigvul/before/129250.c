void GLES2DecoderImpl::DoBindTexImage2DCHROMIUM(
    GLenum target, GLint image_id) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoBindTexImage2DCHROMIUM");

  if (target == GL_TEXTURE_CUBE_MAP) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_ENUM,
        "glBindTexImage2DCHROMIUM", "invalid target");
    return;
  }

  TextureRef* texture_ref =
      texture_manager()->GetTextureInfoForTargetUnlessDefault(&state_, target);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glBindTexImage2DCHROMIUM", "no texture bound");
    return;
  }

  gfx::GLImage* gl_image = image_manager()->LookupImage(image_id);
  if (!gl_image) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glBindTexImage2DCHROMIUM", "no image found with the given ID");
    return;
  }

  {
    ScopedGLErrorSuppressor suppressor(
        "GLES2DecoderImpl::DoBindTexImage2DCHROMIUM", GetErrorState());
    if (!gl_image->BindTexImage(target)) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION,
          "glBindTexImage2DCHROMIUM", "fail to bind image with the given ID");
      return;
    }
  }

  gfx::Size size = gl_image->GetSize();
  texture_manager()->SetLevelInfo(
      texture_ref, target, 0, GL_RGBA, size.width(), size.height(), 1, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, true);
  texture_manager()->SetLevelImage(texture_ref, target, 0, gl_image);
}
