void GLES2DecoderPassthroughImpl::VerifyServiceTextureObjectsExist() {
  resources_->texture_object_map.ForEach(
      [this](GLuint client_id, scoped_refptr<TexturePassthrough> texture) {
        DCHECK_EQ(GL_TRUE, api()->glIsTextureFn(texture->service_id()));
      });
}
