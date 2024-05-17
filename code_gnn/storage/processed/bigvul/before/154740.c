error::Error GLES2DecoderPassthroughImpl::DoIsShader(GLuint shader,
                                                     uint32_t* result) {
  *result = api()->glIsShaderFn(GetShaderServiceID(shader, resources_));
  return error::kNoError;
}
