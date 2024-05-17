void GLES2Implementation::DeleteShaderStub(GLsizei n, const GLuint* shaders) {
  DCHECK_EQ(1, n);
  share_group_->program_info_manager()->DeleteInfo(shaders[0]);
  helper_->DeleteShader(shaders[0]);
}
