error::Error GLES2DecoderPassthroughImpl::DoDetachShader(GLuint program,
                                                         GLuint shader) {
  api()->glDetachShaderFn(GetProgramServiceID(program, resources_),
                          GetShaderServiceID(shader, resources_));
  return error::kNoError;
}
