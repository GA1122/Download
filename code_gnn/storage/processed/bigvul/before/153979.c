void GLES2DecoderImpl::DoCopyTexImage(Texture* texture,
                                      GLenum textarget,
                                      gl::GLImage* image) {
  texture->SetLevelImageState(textarget, 0, Texture::COPIED);
  bool rv = image->CopyTexImage(textarget);
  DCHECK(rv) << "CopyTexImage() failed";
}
