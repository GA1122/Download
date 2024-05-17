void WebGLRenderingContextBase::SynthesizeGLError(
    GLenum error,
    const char* function_name,
    const char* description,
    ConsoleDisplayPreference display) {
  String error_type = GetErrorString(error);
  if (synthesized_errors_to_console_ && display == kDisplayInConsole) {
    String message = String("WebGL: ") + error_type + ": " +
                     String(function_name) + ": " + String(description);
    PrintGLErrorToConsole(message);
  }
  if (!isContextLost()) {
    if (!synthetic_errors_.Contains(error))
      synthetic_errors_.push_back(error);
  } else {
    if (!lost_context_errors_.Contains(error))
      lost_context_errors_.push_back(error);
  }
  probe::didFireWebGLError(canvas(), error_type);
}
