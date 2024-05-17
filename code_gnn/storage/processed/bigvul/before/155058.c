void WebGLRenderingContextBase::shaderSource(WebGLShader* shader,
                                             const String& string) {
  if (!ValidateWebGLProgramOrShader("shaderSource", shader))
    return;
  String string_without_comments = StripComments(string).Result();
  if (!ValidateShaderSource(string_without_comments))
    return;
  shader->SetSource(string);
  WTF::StringUTF8Adaptor adaptor(string_without_comments);
  const GLchar* shader_data = adaptor.data();
  const GLint shader_length = adaptor.size();
  ContextGL()->ShaderSource(ObjectOrZero(shader), 1, &shader_data,
                            &shader_length);
}
