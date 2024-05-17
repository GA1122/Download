PepperVideoRenderer3D::~PepperVideoRenderer3D() {
  if (shader_program_)
    gles2_if_->DeleteProgram(graphics_.pp_resource(), shader_program_);

  STLDeleteElements(&pending_packets_);
}
