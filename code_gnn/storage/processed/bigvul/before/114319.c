WebKit::WebString WebGraphicsContext3DCommandBufferImpl::getShaderSource(
    WebGLId shader) {
  GLint logLength = 0;
  gl_->GetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &logLength);
  if (!logLength)
    return WebKit::WebString();
  scoped_array<GLchar> log(new GLchar[logLength]);
  if (!log.get())
    return WebKit::WebString();
  GLsizei returnedLogLength = 0;
  gl_->GetShaderSource(
      shader, logLength, &returnedLogLength, log.get());
  if (!returnedLogLength)
    return WebKit::WebString();
  DCHECK_EQ(logLength, returnedLogLength + 1);
  WebKit::WebString res =
      WebKit::WebString::fromUTF8(log.get(), returnedLogLength);
  return res;
}