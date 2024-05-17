void PepperVideoRenderer3D::CreateProgram(const char* vertex_shader,
                                          const char* fragment_shader) {
  PP_Resource graphics_3d = graphics_.pp_resource();
  if (shader_program_)
    gles2_if_->DeleteProgram(graphics_3d, shader_program_);

  shader_program_ = gles2_if_->CreateProgram(graphics_3d);
  CreateShaderProgram(GL_VERTEX_SHADER, vertex_shader);
  CreateShaderProgram(GL_FRAGMENT_SHADER, fragment_shader);
  gles2_if_->LinkProgram(graphics_3d, shader_program_);
  gles2_if_->UseProgram(graphics_3d, shader_program_);
  gles2_if_->Uniform1i(
      graphics_3d,
      gles2_if_->GetUniformLocation(graphics_3d, shader_program_, "s_texture"),
      0);
  CheckGLError();

  shader_texcoord_scale_location_ = gles2_if_->GetUniformLocation(
      graphics_3d, shader_program_, "v_scale");

  GLint pos_location = gles2_if_->GetAttribLocation(
      graphics_3d, shader_program_, "a_position");
  GLint tc_location = gles2_if_->GetAttribLocation(
      graphics_3d, shader_program_, "a_texCoord");
  CheckGLError();

  gles2_if_->EnableVertexAttribArray(graphics_3d, pos_location);
  gles2_if_->VertexAttribPointer(graphics_3d, pos_location, 2, GL_FLOAT,
                                 GL_FALSE, 0, 0);
  gles2_if_->EnableVertexAttribArray(graphics_3d, tc_location);
  gles2_if_->VertexAttribPointer(
      graphics_3d, tc_location, 2, GL_FLOAT, GL_FALSE, 0,
      static_cast<float*>(0) + 8);   

  gles2_if_->UseProgram(graphics_3d, 0);

  CheckGLError();
}
