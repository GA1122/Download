void APIENTRY GLDebugMessageCallback(GLenum source,
                                     GLenum type,
                                     GLuint id,
                                     GLenum severity,
                                     GLsizei length,
                                     const GLchar* message,
                                     const GLvoid* user_param) {
  Logger* error_logger = static_cast<Logger*>(const_cast<void*>(user_param));
  LogGLDebugMessage(source, type, id, severity, length, message, error_logger);
}
