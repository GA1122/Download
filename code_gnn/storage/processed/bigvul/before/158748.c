bool GLES2DecoderImpl::SimulateAttrib0(
    const char* function_name, GLuint max_vertex_accessed, bool* simulated) {
  DCHECK(simulated);
  *simulated = false;

  if (gl_version_info().BehavesLikeGLES())
    return true;

  const VertexAttrib* attrib =
      state_.vertex_attrib_manager->GetVertexAttrib(0);
  bool attrib_0_used =
      state_.current_program->GetAttribInfoByLocation(0) != nullptr;
  if (attrib->enabled() && attrib_0_used) {
    return true;
  }

  GLuint num_vertices = max_vertex_accessed + 1;
  uint32_t size_needed = 0;

  if (num_vertices == 0 ||
      !SafeMultiplyUint32(num_vertices, sizeof(Vec4f), &size_needed) ||
      size_needed > 0x7FFFFFFFU) {
    LOCAL_SET_GL_ERROR(GL_OUT_OF_MEMORY, function_name, "Simulating attrib 0");
    return false;
  }

  LOCAL_PERFORMANCE_WARNING(
      "Attribute 0 is disabled. This has significant performance penalty");

  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER(function_name);
  api()->glBindBufferFn(GL_ARRAY_BUFFER, attrib_0_buffer_id_);

  bool new_buffer = static_cast<GLsizei>(size_needed) > attrib_0_size_;
  if (new_buffer) {
    api()->glBufferDataFn(GL_ARRAY_BUFFER, size_needed, nullptr,
                          GL_DYNAMIC_DRAW);
    GLenum error = api()->glGetErrorFn();
    if (error != GL_NO_ERROR) {
      LOCAL_SET_GL_ERROR(
          GL_OUT_OF_MEMORY, function_name, "Simulating attrib 0");
      return false;
    }
  }

  const Vec4& value = state_.attrib_values[0];
  if (new_buffer || (attrib_0_used && (!attrib_0_buffer_matches_value_ ||
                                       !value.Equal(attrib_0_value_)))) {
    Vec4f fvalue(value);
    constexpr GLuint kMaxVerticesPerLoop = 32u << 10;
    const GLuint vertices_per_loop =
        std::min(num_vertices, kMaxVerticesPerLoop);
    std::vector<Vec4f> temp(vertices_per_loop, fvalue);
    for (GLuint offset = 0; offset < num_vertices;) {
      size_t count = std::min(num_vertices - offset, vertices_per_loop);
      api()->glBufferSubDataFn(GL_ARRAY_BUFFER, offset * sizeof(Vec4f),
                               count * sizeof(Vec4f), temp.data());
      offset += count;
    }
    attrib_0_buffer_matches_value_ = true;
    attrib_0_value_ = value;
    attrib_0_size_ = size_needed;
  }

  api()->glVertexAttribPointerFn(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

  if (feature_info_->feature_flags().angle_instanced_arrays)
    api()->glVertexAttribDivisorANGLEFn(0, 0);

  *simulated = true;
  return true;
}
