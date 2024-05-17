bool GLES2Implementation::GetProgramInterfaceivHelper(GLuint program,
                                                      GLenum program_interface,
                                                      GLenum pname,
                                                      GLint* params) {
  bool success = share_group_->program_info_manager()->GetProgramInterfaceiv(
      this, program, program_interface, pname, params);
  GPU_CLIENT_LOG_CODE_BLOCK({
    if (success) {
      GPU_CLIENT_LOG("  0: " << *params);
    }
  });
  return success;
}
