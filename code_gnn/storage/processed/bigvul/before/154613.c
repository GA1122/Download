error::Error GLES2DecoderPassthroughImpl::DoCreateProgram(GLuint client_id) {
  return CreateHelper(client_id, &resources_->program_id_map,
                      [this]() { return api()->glCreateProgramFn(); });
}
