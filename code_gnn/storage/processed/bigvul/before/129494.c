ScopedGLErrorSuppressor::ScopedGLErrorSuppressor(
    const char* function_name, ErrorState* error_state)
    : function_name_(function_name),
      error_state_(error_state) {
  ERRORSTATE_COPY_REAL_GL_ERRORS_TO_WRAPPER(error_state_, function_name_);
}
