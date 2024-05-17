error::Error GLES2DecoderImpl::HandleDrawElementsInstancedANGLE(
    uint32 immediate_data_size, const cmds::DrawElementsInstancedANGLE& c) {
  if (!features().angle_instanced_arrays) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glDrawElementsInstancedANGLE", "function not available");
    return error::kNoError;
  }
  return DoDrawElements("glDrawElementsInstancedANGLE",
                        true,
                        static_cast<GLenum>(c.mode),
                        static_cast<GLsizei>(c.count),
                        static_cast<GLenum>(c.type),
                        static_cast<int32>(c.index_offset),
                        static_cast<GLsizei>(c.primcount));
}
