bool GLES2DecoderImpl::DoIsTexture(GLuint client_id) {
  const TextureRef* texture_ref = GetTexture(client_id);
  return texture_ref && texture_ref->texture()->IsValid();
}
