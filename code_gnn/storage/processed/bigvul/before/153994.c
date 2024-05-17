void GLES2DecoderImpl::DoEndSharedImageAccessDirectCHROMIUM(GLuint client_id) {
  TextureRef* texture_ref = GetTexture(client_id);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "DoBeginSharedImageAccessCHROMIUM",
                       "invalid texture id");
    return;
  }

  SharedImageRepresentationGLTexture* shared_image =
      texture_ref->shared_image();
  if (!shared_image) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "DoEndSharedImageAccessCHROMIUM",
                       "bound texture is not a shared image");
    return;
  }

  shared_image->EndAccess();
}
