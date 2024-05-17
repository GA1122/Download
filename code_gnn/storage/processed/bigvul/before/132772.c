void PepperVideoRenderer3D::CheckGLError() {
  GLenum error = gles2_if_->GetError(graphics_.pp_resource());
  CHECK_EQ(error, static_cast<GLenum>(GL_NO_ERROR)) << "GL error: " << error;
}
