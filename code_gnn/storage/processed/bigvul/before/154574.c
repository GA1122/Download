error::Error GLES2DecoderPassthroughImpl::DoBindVertexArrayOES(GLuint array) {
  api()->glBindVertexArrayOESFn(
      GetVertexArrayServiceID(array, &vertex_array_id_map_));
  return error::kNoError;
}
