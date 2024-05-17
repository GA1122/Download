GLES2DecoderPassthroughImpl::DoBeginSharedImageAccessDirectCHROMIUM(
    GLuint client_id,
    GLenum mode) {
  if (mode != GL_SHARED_IMAGE_ACCESS_MODE_READWRITE_CHROMIUM &&
      mode != GL_SHARED_IMAGE_ACCESS_MODE_READ_CHROMIUM) {
    InsertError(GL_INVALID_ENUM, "unrecognized access mode");
    return error::kNoError;
  }

  auto found = resources_->texture_shared_image_map.find(client_id);
  if (found == resources_->texture_shared_image_map.end()) {
    InsertError(GL_INVALID_OPERATION, "texture is not a shared image");
    return error::kNoError;
  }

  SharedImageRepresentationGLTexturePassthrough* shared_image =
      found->second.get();
  if (!shared_image->BeginAccess(mode)) {
    InsertError(GL_INVALID_OPERATION, "unable to begin access");
    return error::kNoError;
  }

  return error::kNoError;
}
