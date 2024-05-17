void GLES2Implementation::ProgramPathFragmentInputGenCHROMIUM(
    GLuint program,
    GLint location,
    GLenum gen_mode,
    GLint components,
    const GLfloat* coeffs) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix()
                     << "] glProgramPathFragmentInputGenCHROMIUM(" << program
                     << ", " << gen_mode << ", " << components << ", " << coeffs
                     << ")");

  uint32_t coeffs_per_component =
      GLES2Util::GetCoefficientCountForGLPathFragmentInputGenMode(gen_mode);

  if (components <= 0 || components > 4 || gen_mode == GL_NONE ||
      coeffs_per_component == 0 || location == -1) {
    helper_->ProgramPathFragmentInputGenCHROMIUM(program, location, gen_mode,
                                                 components, 0, 0);
  } else {
    DCHECK(coeffs_per_component > 0 && coeffs_per_component <= 4);
    DCHECK(components > 0 && components <= 4);
    uint32_t coeffs_size = sizeof(GLfloat) * coeffs_per_component * components;

    ScopedTransferBufferPtr buffer(coeffs_size, helper_, transfer_buffer_);
    if (!buffer.valid() || buffer.size() < coeffs_size) {
      SetGLError(GL_OUT_OF_MEMORY, "glProgramPathFragmentInputGenCHROMIUM",
                 "no room in transfer buffer");
      return;
    }

    DCHECK_GT(coeffs_size, 0U);
    unsigned char* addr = static_cast<unsigned char*>(buffer.address());
    memcpy(addr, coeffs, coeffs_size);

    helper_->ProgramPathFragmentInputGenCHROMIUM(program, location, gen_mode,
                                                 components, buffer.shm_id(),
                                                 buffer.offset());
  }
  CheckGLError();
}
