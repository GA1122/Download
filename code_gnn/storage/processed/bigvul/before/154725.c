error::Error GLES2DecoderPassthroughImpl::DoGetVertexAttribPointerv(
    GLuint index,
    GLenum pname,
    GLsizei bufsize,
    GLsizei* length,
    GLuint* pointer) {
  std::array<void*, 1> temp_pointers{{nullptr}};
  GLsizei temp_length = 0;
  api()->glGetVertexAttribPointervRobustANGLEFn(
      index, pname, static_cast<GLsizei>(temp_pointers.size()), &temp_length,
      temp_pointers.data());
  DCHECK(temp_length >= 0 &&
         temp_length <= static_cast<GLsizei>(temp_pointers.size()) &&
         temp_length <= bufsize);
  for (GLsizei ii = 0; ii < temp_length; ii++) {
    pointer[ii] =
        static_cast<GLuint>(reinterpret_cast<uintptr_t>(temp_pointers[ii]));
  }
  *length = temp_length;
  return error::kNoError;
}
