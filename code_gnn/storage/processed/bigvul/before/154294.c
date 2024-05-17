error::Error GLES2DecoderImpl::HandleVertexAttribDivisorANGLE(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::VertexAttribDivisorANGLE& c =
      *static_cast<const volatile gles2::cmds::VertexAttribDivisorANGLE*>(
          cmd_data);
  if (!features().angle_instanced_arrays)
    return error::kUnknownCommand;

  GLuint index = c.index;
  GLuint divisor = c.divisor;
  if (index >= group_->max_vertex_attribs()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE,
        "glVertexAttribDivisorANGLE", "index out of range");
    return error::kNoError;
  }

  state_.vertex_attrib_manager->SetDivisor(
      index,
      divisor);
  api()->glVertexAttribDivisorANGLEFn(index, divisor);
  return error::kNoError;
}
