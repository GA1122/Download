error::Error GLES2DecoderImpl::HandleDrawArraysInstancedANGLE(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::DrawArraysInstancedANGLE& c =
      *static_cast<const volatile gles2::cmds::DrawArraysInstancedANGLE*>(
          cmd_data);
  if (!features().angle_instanced_arrays)
    return error::kUnknownCommand;

  GLint first = static_cast<GLint>(c.first);
  GLsizei count = static_cast<GLsizei>(c.count);
  GLsizei primcount = static_cast<GLsizei>(c.primcount);
  return DoMultiDrawArrays("glDrawArraysInstancedANGLE", true,
                           static_cast<GLenum>(c.mode), &first, &count,
                           &primcount, 1);
}
