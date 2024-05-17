error::Error GLES2DecoderPassthroughImpl::DoCompileShader(GLuint shader) {
  api()->glCompileShaderFn(GetShaderServiceID(shader, resources_));
  return error::kNoError;
}
