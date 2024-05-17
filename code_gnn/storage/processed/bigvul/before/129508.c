bool GLES2DecoderImpl::SimulateAttrib0(
    const char* function_name, GLuint max_vertex_accessed, bool* simulated) {
  DCHECK(simulated);
  *simulated = false;

  if (gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2)
    return true;

  const VertexAttrib* attrib =
      state_.vertex_attrib_manager->GetVertexAttrib(0);
  bool attrib_0_used =
      state_.current_program->GetAttribInfoByLocation(0) != NULL;
  if (attrib->enabled() && attrib_0_used) {
    return true;
  }

  GLuint num_vertices = max_vertex_accessed + 1;
  uint32 size_needed = 0;

  if (num_vertices == 0 ||
      !SafeMultiplyUint32(num_vertices, sizeof(Vec4), &size_needed) ||
      size_needed > 0x7FFFFFFFU) {
    LOCAL_SET_GL_ERROR(GL_OUT_OF_MEMORY, function_name, "Simulating attrib 0");
    return false;
  }

  LOCAL_PERFORMANCE_WARNING(
      "Attribute 0 is disabled. This has signficant performance penalty");

  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER(function_name);
  glBindBuffer(GL_ARRAY_BUFFER, attrib_0_buffer_id_);

  bool new_buffer = static_cast<GLsizei>(size_needed) > attrib_0_size_;
  if (new_buffer) {
    glBufferData(GL_ARRAY_BUFFER, size_needed, NULL, GL_DYNAMIC_DRAW);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      LOCAL_SET_GL_ERROR(
          GL_OUT_OF_MEMORY, function_name, "Simulating attrib 0");
      return false;
    }
  }

  const Vec4& value = state_.attrib_values[0];
  if (new_buffer ||
      (attrib_0_used &&
       (!attrib_0_buffer_matches_value_ ||
        (value.v[0] != attrib_0_value_.v[0] ||
         value.v[1] != attrib_0_value_.v[1] ||
         value.v[2] != attrib_0_value_.v[2] ||
         value.v[3] != attrib_0_value_.v[3])))) {
    std::vector<Vec4> temp(num_vertices, value);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size_needed, &temp[0].v[0]);
    attrib_0_buffer_matches_value_ = true;
    attrib_0_value_ = value;
    attrib_0_size_ = size_needed;
  }

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

  if (attrib->divisor())
    glVertexAttribDivisorANGLE(0, 0);

  *simulated = true;
  return true;
}
