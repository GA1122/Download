void PepperVideoRenderer3D::CreateShaderProgram(int type, const char* source) {
  int size = strlen(source);
  GLuint shader = gles2_if_->CreateShader(graphics_.pp_resource(), type);
  gles2_if_->ShaderSource(graphics_.pp_resource(), shader, 1, &source, &size);
  gles2_if_->CompileShader(graphics_.pp_resource(), shader);
  gles2_if_->AttachShader(graphics_.pp_resource(), shader_program_, shader);
  gles2_if_->DeleteShader(graphics_.pp_resource(), shader);
}
