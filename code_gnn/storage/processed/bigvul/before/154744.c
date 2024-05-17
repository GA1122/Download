error::Error GLES2DecoderPassthroughImpl::DoIsVertexArrayOES(GLuint array,
                                                             uint32_t* result) {
  *result = api()->glIsVertexArrayOESFn(
      GetVertexArrayServiceID(array, &vertex_array_id_map_));
  return error::kNoError;
}
