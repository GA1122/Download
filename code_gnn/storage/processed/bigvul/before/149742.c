bool IsGL_REDSupportedOnFBOs() {
  DCHECK(glGetError() == GL_NO_ERROR);
  GLint fb_binding = 0;
  GLint tex_binding = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb_binding);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex_binding);

  GLuint textureId = 0;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  GLubyte data[1] = {0};
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED_EXT, 1, 1, 0, GL_RED_EXT,
               GL_UNSIGNED_BYTE, data);
  GLuint textureFBOID = 0;
  glGenFramebuffersEXT(1, &textureFBOID);
  glBindFramebufferEXT(GL_FRAMEBUFFER, textureFBOID);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                            textureId, 0);
  bool result =
      glCheckFramebufferStatusEXT(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
  glDeleteFramebuffersEXT(1, &textureFBOID);
  glDeleteTextures(1, &textureId);

  glBindFramebufferEXT(GL_FRAMEBUFFER, static_cast<GLuint>(fb_binding));
  glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(tex_binding));

  DCHECK(glGetError() == GL_NO_ERROR);

  return result;
}
