void RenderingHelper::DeleteTexture(GLuint texture_id) {
  glDeleteTextures(1, &texture_id);
  CHECK_EQ(static_cast<int>(glGetError()), GL_NO_ERROR);
}
