void GLES2DecoderImpl::DoBeginSharedImageAccessDirectCHROMIUM(GLuint client_id,
                                                              GLenum mode) {
  TextureRef* texture_ref = GetTexture(client_id);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "DoBeginSharedImageAccessCHROMIUM",
                       "invalid texture id");
    return;
  }

  SharedImageRepresentationGLTexture* shared_image =
      texture_ref->shared_image();
  if (!shared_image) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "DoBeginSharedImageAccessCHROMIUM",
                       "bound texture is not a shared image");
    return;
  }

  if (!shared_image->BeginAccess(mode)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "DoBeginSharedImageAccessCHROMIUM",
                       "Unable to begin access");
    return;
  }
}
