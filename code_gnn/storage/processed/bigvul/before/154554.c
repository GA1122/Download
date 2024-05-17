error::Error GLES2DecoderPassthroughImpl::DoAttachShader(GLuint program,
                                                         GLuint shader) {
  api()->glAttachShaderFn(GetProgramServiceID(program, resources_),
                          GetShaderServiceID(shader, resources_));
  return error::kNoError;
}
