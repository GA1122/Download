error::Error GLES2DecoderImpl::HandleDrawElementsInstancedANGLE(
    uint32 immediate_data_size, const gles2::DrawElementsInstancedANGLE& c) {
  if (!feature_info_->feature_flags().angle_instanced_arrays) {
    SetGLError(GL_INVALID_OPERATION,
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
