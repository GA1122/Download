error::Error GLES2DecoderPassthroughImpl::DoSetColorSpaceMetadataCHROMIUM(
    GLuint texture_id,
    gfx::ColorSpace color_space) {
  scoped_refptr<TexturePassthrough> passthrough_texture = nullptr;
  if (!resources_->texture_object_map.GetServiceID(texture_id,
                                                   &passthrough_texture) ||
      passthrough_texture == nullptr) {
    InsertError(GL_INVALID_VALUE, "unknown texture.");
    return error::kNoError;
  }

  scoped_refptr<gl::GLImage> image =
      passthrough_texture->GetLevelImage(passthrough_texture->target(), 0);
  if (image == nullptr) {
    InsertError(GL_INVALID_VALUE, "no image associated with texture.");
    return error::kNoError;
  }

  image->SetColorSpace(color_space);
  return error::kNoError;
}
