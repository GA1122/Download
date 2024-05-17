void APIENTRY PassthroughGLDebugMessageCallback(GLenum source,
                                                GLenum type,
                                                GLuint id,
                                                GLenum severity,
                                                GLsizei length,
                                                const GLchar* message,
                                                const GLvoid* user_param) {
  DCHECK(user_param != nullptr);
  GLES2DecoderPassthroughImpl* command_decoder =
      static_cast<GLES2DecoderPassthroughImpl*>(const_cast<void*>(user_param));
  command_decoder->OnDebugMessage(source, type, id, severity, length, message);
  LogGLDebugMessage(source, type, id, severity, length, message,
                    command_decoder->GetLogger());
}
