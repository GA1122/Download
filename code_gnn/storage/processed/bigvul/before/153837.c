void GLES2Implementation::UniformBlockBinding(GLuint program,
                                              GLuint index,
                                              GLuint binding) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glUniformBlockBinding(" << program
                     << ", " << index << ", " << binding << ")");
  share_group_->program_info_manager()->UniformBlockBinding(this, program,
                                                            index, binding);
  helper_->UniformBlockBinding(program, index, binding);
  CheckGLError();
}
